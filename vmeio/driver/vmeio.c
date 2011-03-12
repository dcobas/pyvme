/**
 * =================================================
 * Implement driver to do basic VME IO
 * Julian Lewis BE/CO/HT Tue 19th October 2010
 */

#include <asm/io.h>
#include <asm/uaccess.h>
#include <linux/fs.h>
#include <linux/interrupt.h>

#include "vmebus.h"
#include "vmeio.h"

#define TSI148_LCSR_DSTA_DON (1<<25)	/* DMA done */

/*
 * ======================================================================
 * Static memory
 */

static int vmeio_major = 0;

#define PFX DRIVER_NAME ": "

MODULE_AUTHOR("Julian Lewis BE/CO/HT CERN");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Raw IO to VME");
MODULE_SUPPORTED_DEVICE("Any VME device");

/*
 * ==============================
 * Module parameter storage area
 * Indexed by minor device number
 */

static unsigned int lun_num;
static long lun[DRV_MAX_DEVICES];
module_param_array(lun, long, &lun_num, S_IRUGO);
MODULE_PARM_DESC(lun, "Logical unit numbers");

static unsigned int base_address1_num;
static long base_address1[DRV_MAX_DEVICES];
module_param_array(base_address1, long, &base_address1_num, S_IRUGO);
MODULE_PARM_DESC(base_address1, "First map base addresses");

static unsigned int base_address2_num;
static long base_address2[DRV_MAX_DEVICES];
module_param_array(base_address2, long, &base_address2_num, S_IRUGO);
MODULE_PARM_DESC(base_address2, "Second map base addresses");

static unsigned int vector_num;
static long vector[DRV_MAX_DEVICES];
module_param_array(vector, long, &vector_num, S_IRUGO);
MODULE_PARM_DESC(vector, "Interrupt vectors");

/* Single value parameter handling */
/* Usually the same for each lun.  */

static long level;
module_param(level, long, S_IRUGO);
MODULE_PARM_DESC(level, "Interrupt levels");

static long am1;
module_param(am1, long, S_IRUGO);
MODULE_PARM_DESC(am1, "First VME address modifier");

static long am2;
module_param(am2, long, S_IRUGO);
MODULE_PARM_DESC(am2, "Second VME address modifier");

static long data_width1;
module_param(data_width1, long, S_IRUGO);
MODULE_PARM_DESC(data_width1, "First data width 1,2,4,8 bytes");

static long data_width2;
module_param(data_width2, long, S_IRUGO);
MODULE_PARM_DESC(data_width2, "Second data width 1,2,4,8 bytes");

static long size1;
module_param(size1, long, S_IRUGO);
MODULE_PARM_DESC(size1, "First mapping size in bytes");

static long size2;
module_param(size2, long, S_IRUGO);
MODULE_PARM_DESC(size2, "Second mapping size in bytes");

static long isrc;
module_param(isrc, long, S_IRUGO);
MODULE_PARM_DESC(isrc, "Location of interrupt source reg in base_address1");

/*
 * This structure describes all the relevant information about a mapping
 */
struct vmeio_map {
	unsigned long	base_address;
	unsigned long	address_modifier;
	unsigned long	data_width;
	unsigned long	mapping_size;
	void		*vaddr;		/* NULL if not mapped */
};

/*
 * vmeio device descriptor:
 *	maps[max_maps]		array of VME mappings
 *
 *	isrfl			1 if interrupt handler installed
 *	isrc			offset of int source reg in map0
 *	isr_source_address	interrupt source reg address
 *	isr_source_mask		result of the read
 *
 *	queue			interrupt waits
 *	timeout			timeout value for wait queue
 *	icnt			interrupt counter
 *
 *	debug			debug level
 */

#define MAX_MAPS	2

struct vmeio_device {
	int			lun;
	struct vmeio_map	maps[MAX_MAPS];

	int			vector;
	int			level;
	unsigned		isrc;
	int			isrfl;
	void			*isr_source_address;
	int			isr_source_mask;

	wait_queue_head_t	queue;
	int			timeout;
	int			icnt;

	int			debug;
};

static struct vmeio_device devices[DRV_MAX_DEVICES];

struct file_operations vmeio_fops;

/* ================= */

int check_minor(long num)
{
	if (num < 0 || num >= DRV_MAX_DEVICES) {
		printk(PFX "minor:%d ", (int) num);
		printk("BAD not in [0..%d]\n", DRV_MAX_DEVICES - 1);
		return 0;
	}
	return 1;
}

/*
 * =========================================================
 * Interrupt service routine
 * =========================================================
 */

static irqreturn_t vmeio_irq(void *arg)
{
	struct vmeio_device *dev = arg;
	long data;

	if (dev->isr_source_address) {
		unsigned long data_width = dev->maps[0].data_width;
		if (data_width == 4)
			data = ioread32be(dev->isr_source_address);
		else if (data_width == 2)
			data = ioread16be(dev->isr_source_address);
		else
			data = ioread8(dev->isr_source_address);
		dev->isr_source_mask = data;
	}
	dev->icnt++;
	wake_up(&dev->queue);
	return IRQ_HANDLED;
}

/* ==================== */

void *find_mapping(int vme, int amd, int dwd, int size) {

	unsigned long vmeaddr;
	struct pdparam_master param;
	char *msg;

	if (!(vme && amd && size && dwd)) return NULL;

	param.iack = 1;                 /* no iack */
	param.rdpref = 0;               /* no VME read prefetch option */
	param.wrpost = 0;               /* no VME write posting option */
	param.swap = 1;                 /* VME auto swap option */
	param.dum[0] = VME_PG_SHARED;   /* window is sharable */
	param.dum[1] = 0;               /* XPC ADP-type */
	param.dum[2] = 0;               /* window is sharable */

	vmeaddr = find_controller(vme, size, amd, 0, dwd, &param);

	if (vmeaddr == -1UL) {
		msg = "ERROR:NotMapped";
		vmeaddr = 0;
	} else {
		msg = "OK:Mapped";
	}
	printk(PFX "%s:Address:0x%X Size:0x%X"
			":AddrMod:0x%X DWidth:0x%X:VirtAddr:0x%lX\n",
			msg, vme, size, amd, dwd, vmeaddr);
	return (void *)vmeaddr;
}

static void vmeio_map_init(struct vmeio_map *map,
		int vme, int size, int amd, int dwd)
{
	map->base_address	= vme;
	map->mapping_size	= size;
	map->address_modifier	= amd;
	map->data_width		= dwd;
	map->vaddr		= NULL;
}

static void vmeio_map_register(struct vmeio_map *map)
{
	map->vaddr = find_mapping(map->base_address, map->address_modifier,
				map->data_width, map->mapping_size);
}

static void vmeio_map_unregister(struct vmeio_map *map)
{
	if (map->base_address)
		return_controller(map->base_address, map->mapping_size);
}


void register_isr(struct vmeio_device *dev, unsigned vector, unsigned level)
{
	int err;

	err = vme_intset(vector, (int (*)(void *)) vmeio_irq, dev, 0);
	dev->isrfl = !(err < 0);
	printk(PFX "ISR:Level:0x%X Vector:0x%X:%s\n",
		level, vector,
		(err < 0) ? "ERROR:NotRegistered" : "OK:Registered");
}

void register_int_source(struct vmeio_device *dev, void *map, unsigned offset)
{
	dev->isr_source_address = dev->maps[0].vaddr + dev->isrc;
	printk("SourceRegister:0x%p", dev->isr_source_address);
}

/*
 * =====================================================
 * Install
 * =====================================================
 */

static int check_module_params(void)
{
	if (lun_num <= 0 || lun_num > DRV_MAX_DEVICES) {
		printk(PFX "Fatal:No logical units defined.\n");
		return -EACCES;
	}

	/* Vector parameters must all be the same size */

	if (vector_num != lun_num && vector_num != 0) {
		printk(PFX "Fatal:Missing interrupt vector.\n");
		return -EACCES;
	}

	if (base_address1_num != lun_num && base_address1_num != 0) {
		printk(PFX "Fatal:Missing first base address.\n");
		return -EACCES;
	}
	if (base_address2_num != lun_num && base_address2_num != 0) {
		printk(PFX "Fatal:Missing second base address.\n");
		return -EACCES;
	}
	return 0;
}

int vmeio_install(void)
{
	int i, cc;

	if ((cc = check_module_params()) != 0)
		return cc;

	/* Build module contexts */

	for (i = 0; i < lun_num; i++) {
		struct vmeio_device *dev = &devices[i];

		memset(dev, 0, sizeof(*dev));

		dev->lun = lun[i];

		vmeio_map_init(&dev->maps[0], base_address1[i], size1, am1, data_width1);
		vmeio_map_init(&dev->maps[1], base_address2[i], size2, am2, data_width2);

		dev->isrc = isrc;
		dev->level  = level;
		dev->vector  = vector[i];

		init_waitqueue_head(&dev->queue);
	}

	/* Register driver */
	cc = register_chrdev(vmeio_major, DRIVER_NAME, &vmeio_fops);
	if (cc < 0) {
		printk(PFX "Fatal:Error from register_chrdev [%d]\n", cc);
		return cc;
	}
	if (vmeio_major == 0)
		vmeio_major = cc;	/* dynamic */

	/* Create VME mappings and register ISRs */

	for (i = 0; i < lun_num; i++) {
		struct vmeio_device *dev = &devices[i];

		dev->debug = DEBUG;
		dev->timeout = msecs_to_jiffies(TIMEOUT);
		dev->icnt = 0;

		printk(PFX "Mapping:Logical unit:%d\n", dev->lun);

		vmeio_map_register(&dev->maps[0]);
		vmeio_map_register(&dev->maps[1]);

		if (dev->level && dev->vector) {
			register_isr(dev, dev->vector, dev->level);
			/* This will be eventually removed */
			register_int_source(dev, dev->maps[0].vaddr, dev->isrc);
		}
	}
	return 0;
}

/* ==================== */

void unregister_module(struct vmeio_device *dev)
{
	if (dev->vector)
		vme_intclr(dev->vector, NULL);
	vmeio_map_unregister(&dev->maps[0]);
	vmeio_map_unregister(&dev->maps[1]);
}

/*
 * =====================================================
 * Uninstall the driver
 * =====================================================
 */

void vmeio_uninstall(void)
{
	int i;

	for (i = 0; i < lun_num; i++) {
		unregister_module(&devices[i]);
	}
	unregister_chrdev(vmeio_major, DRIVER_NAME);
}

/*
 * =====================================================
 * Open
 * =====================================================
 */

int vmeio_open(struct inode *inode, struct file *filp)
{
	long num;

	num = MINOR(inode->i_rdev);
	if (!check_minor(num))
		return -EACCES;

	return 0;
}

/*
 * =====================================================
 * Close
 * =====================================================
 */

int vmeio_close(struct inode *inode, struct file *filp)
{
	long num;

	num = MINOR(inode->i_rdev);
	if (!check_minor(num))
		return -EACCES;

	return 0;
}

/*
 * =====================================================
 * Read
 * =====================================================
 */

ssize_t vmeio_read(struct file * filp, char *buf, size_t count,
		   loff_t * f_pos)
{
	int cc;
	long minor;
	struct inode *inode;

	struct vmeio_read_buf_s rbuf;
	struct vmeio_device *dev;
	int icnt;

	inode = filp->f_dentry->d_inode;
	minor = MINOR(inode->i_rdev);
	if (!check_minor(minor))
		return -EACCES;
	dev = &devices[minor];

	if (dev->debug) {
		printk(PFX "read:count:%d minor:%d\n", 
		       count, (int) minor);
		if (dev->debug > 1) {
			printk(PFX "read:timout:%d\n", dev->timeout);
		}
	}

	if (count < sizeof(rbuf)) {
		if (dev->debug) {
			printk(PFX "read:Access error buffer too small\n");
		}
		return -EACCES;
	}

	icnt = dev->icnt;
	if (dev->timeout) {
		cc = wait_event_interruptible_timeout(dev->queue,
						      icnt != dev->icnt,
						      dev->timeout);
	} else {
		cc = wait_event_interruptible(dev->queue,
					      icnt != dev->icnt);
	}

	if (dev->debug > 2) {
		printk(PFX "wait_event:returned:%d\n", cc);
	}

	if (cc == -ERESTARTSYS) {
		printk(PFX "vmeio_read:interrupted by signal\n");
		return cc;
	}
	if (cc == 0 && dev->timeout)
		return -ETIME;	/* Timer expired */
	if (cc < 0)
		return cc;	/* Error */

	rbuf.logical_unit = dev->lun;
	rbuf.interrupt_mask = dev->isr_source_mask;
	rbuf.interrupt_count = dev->icnt;

	cc = copy_to_user(buf, &rbuf, sizeof(rbuf));
	if (cc != 0) {
		printk(PFX "Can't copy to user space:cc=%d\n", cc);
		return -EACCES;
	}
	return sizeof(struct vmeio_read_buf_s);
}

/*
 * =====================================================
 * Write
 * Used to simulate interrupts
 * =====================================================
 */

ssize_t vmeio_write(struct file * filp, const char *buf, size_t count,
		    loff_t * f_pos)
{
	long minor;
	struct vmeio_device *dev;
	struct inode *inode;
	int cc, mask;

	inode = filp->f_dentry->d_inode;
	minor = MINOR(inode->i_rdev);
	if (!check_minor(minor))
		return -EACCES;
	dev = &devices[minor];

	if (count >= sizeof(int)) {
		cc = copy_from_user(&mask, buf, sizeof(int));
		if (cc != 0) {
			printk(PFX "write:Error:%d could not copy from user\n", cc);
			return -EACCES;
		}
	}

	if (dev->debug) {
		printk(PFX "write:count:%d minor:%d mask:0x%X\n",
		       count, (int) minor, mask);
	}

	dev->isr_source_mask = mask;
	dev->icnt++;
	wake_up(&dev->queue);
	return sizeof(int);
}

/*
 * =====================================================
 * Ioctl
 * =====================================================
 */

#define VME_NO_ADDR_INCREMENT 1
#define DMA_BLOCK_SIZE        4096
#define SAMPLES_IN_DMA_BLOCK  2048

/*
 * ====================================================================
 * Debug routines
 */

static char *ioctl_names[vmeioLAST - vmeioFIRST] = {
	"Unknown IOCTL number",
	"SET_DEBUG",
	"GET_DEBUG",
	"GET_VERSION",
	"SET_TIMEOUT",
	"GET_TIMEOUT",
	"GET_DEVICE",
	"RAW_READ",
	"RAW_WRITE",
	"RAW_READ_DMA",
	"RAW_WRITE_DMA",
	"SET_DEVICE"
};

static void debug_ioctl(int ionr, int iodr, int iosz, void *arg, long num,
			int dlevel)
{
	int c;
	int *iargp = arg;

	if (dlevel <= 0)
		return;

	printk(PFX "debug_ioctl:ionr:%d", ionr);
	if (ionr <= vmeioFIRST || ionr >= vmeioLAST) {
		printk(" BAD:");
	} else {
		c = ionr - vmeioFIRST;
		printk(" %s:", ioctl_names[c]);
	}

	printk(" iodr:%d:", iodr);
	if (iodr & _IOC_WRITE)
		printk("WR:");
	if (iodr & _IOC_READ)
		printk("RD:");

	if (arg)
		c = *iargp;
	else
		c = 0;
	printk(" iosz:%d arg:0x%p[%d] minor:%d\n", iosz, arg, c,
	       (int) num);
}


static void vmeio_set_debug(struct vmeio_device *dev, int *debug)
{
	dev->debug = *debug;
}

static void vmeio_get_debug(struct vmeio_device *dev, int *debug)
{
	*debug = dev->debug;
}

static void vmeio_get_version(int *version)
{
	*version = COMPILE_TIME;
}

static void vmeio_set_timeout(struct vmeio_device *dev, int *timeout)
{
	dev->timeout = msecs_to_jiffies(*timeout);
}

static void vmeio_get_timeout(struct vmeio_device *dev, int *timeout)
{
	*timeout = jiffies_to_msecs(dev->timeout);
}

static void vmeio_get_device(struct vmeio_device *dev,
		struct vmeio_get_mapping_s *mapping)
{
	struct vmeio_map *map0 = &dev->maps[0];
	struct vmeio_map *map1 = &dev->maps[1];

	mapping->lun	= dev->lun;
	mapping->level	= dev->level;
	mapping->vector	= dev->vector;
	mapping->isrc	= dev->isrc;

	mapping->am1	= map0->address_modifier;
	mapping->data_width1	= map0->data_width;
	mapping->base_address1	= map0->base_address;
	mapping->size1	= map0->mapping_size;

	mapping->am2	= map1->address_modifier;
	mapping->data_width2	= map1->data_width;
	mapping->base_address2	= map1->base_address;
	mapping->size2	= map1->mapping_size;
}

static void vmeio_set_device(struct vmeio_device *dev,
		struct vmeio_get_mapping_s *mapping)
{
	struct vmeio_map *map0 = &dev->maps[0];
	struct vmeio_map *map1 = &dev->maps[1];

	vmeio_map_unregister(map0);
	vmeio_map_init(map0, mapping->base_address1, mapping->size1, mapping->am1, mapping->data_width1);
	vmeio_map_unregister(map1);
	vmeio_map_init(map1, mapping->base_address2, mapping->size2, mapping->am2, mapping->data_width2);
	vmeio_map_register(map0);
	vmeio_map_register(map1);
}

static int do_raw_dma(struct vmeio_dma_op *request)
{
	struct vme_dma dma_desc;
	unsigned int bu, bl;
	int cc;
	unsigned int haddr;

	bl = (unsigned long)request->buffer;
	bu = 0;

	memset(&dma_desc, 0, sizeof(dma_desc));

	dma_desc.dir = request->direction;
	dma_desc.novmeinc = 0;
	dma_desc.length = request->byte_length;

	dma_desc.ctrl.pci_block_size = VME_DMA_BSIZE_4096;
	dma_desc.ctrl.pci_backoff_time = VME_DMA_BACKOFF_0;
	dma_desc.ctrl.vme_block_size = VME_DMA_BSIZE_4096;
	dma_desc.ctrl.vme_backoff_time = VME_DMA_BACKOFF_0;

	dma_desc.dst.data_width = request->data_width;
	dma_desc.dst.am = request->am;
	dma_desc.src.data_width = request->data_width;
	dma_desc.src.am = request->am;

	haddr = request->address;

	if (request->direction == VME_DMA_TO_DEVICE) {
		dma_desc.src.addrl = bl;
		dma_desc.src.addru = bu;
		dma_desc.dst.addrl = haddr;
	} else {
		dma_desc.src.addrl = haddr;
		dma_desc.dst.addrl = bl;
		dma_desc.dst.addru = bu;
	}

	if ((cc = vme_do_dma(&dma_desc)) < 0)
		return cc;

	if (!(dma_desc.status & TSI148_LCSR_DSTA_DON)) {
		printk(PFX "DMA:NotDone:Status:0x%X\n", dma_desc.status);
		return -EIO;
	}

	return 0;
}

static int raw_dma(struct vmeio_device *dev,
	struct vmeio_riob_s *riob, enum vme_dma_dir direction)
{
	struct vmeio_map *map = &dev->maps[riob->mapnum];
	struct vmeio_dma_op req;

	req.am = map->address_modifier;
	req.data_width = 8*map->data_width;
	req.address = map->base_address + riob->offset;
	req.byte_length = riob->bsize;
	req.buffer = riob->buffer;
	req.direction = direction;

	return do_raw_dma(&req);
}

union vmeio_word {
	int	width4;
	short	width2;
	char	width1;
};


static int raw_read(struct vmeio_device *dev, struct vmeio_riob_s *riob)
{
	struct vmeio_map *mapx = &dev->maps[riob->mapnum-1];
	int dwidth = mapx->data_width;
	int i, j, cc;
	char *map, *iob;

	if (riob->bsize > vmeioMAX_BUF)
		return -E2BIG;
	iob = kmalloc(riob->bsize, GFP_KERNEL);
	if (!iob)
		return -ENOMEM;
	if ((map = mapx->vaddr) == NULL) {
		kfree(iob);
		return -ENODEV;
	}
	if (dev->debug > 1) {
		printk("RAW:READ:win:%d map:0x%p offs:0x%X amd:0x%2lx dwd:%d len:%d\n",
		     riob->mapnum, map, riob->offset,
		     mapx->address_modifier, dwidth,
		     riob->bsize);
	}

	for (i = 0, j = riob->offset; i < riob->bsize; i += dwidth, j += dwidth) {
		union vmeio_word *dst = (void *)&iob[i];
		if (dwidth == 4)
			dst->width4 = ioread32be(&map[j]);
		else if (dwidth == 2)
			dst->width2 = ioread16be(&map[j]);
		else
			dst->width1 = ioread8(&map[j]);
	}
	cc = copy_to_user(riob->buffer, iob, riob->bsize);
	kfree(iob);
	if (cc)
		return -EACCES;
	return 0;
}

static int raw_write(struct vmeio_device *dev, struct vmeio_riob_s *riob)
{
	struct vmeio_map *mapx = &dev->maps[riob->mapnum-1];	
	int dwidth = mapx->data_width;
	int i, j, cc;
	char *map, *iob;

	if (riob->bsize > vmeioMAX_BUF)
		return -E2BIG;
	iob = kmalloc(riob->bsize, GFP_KERNEL);
	if (!iob)
		return -ENOMEM;
	if ((map = mapx->vaddr) == NULL) {
		kfree(iob);
		return -ENODEV;
	}

	cc = copy_from_user(iob, riob->buffer, riob->bsize);
	if (cc < 0) {
		kfree(iob);
		return -EACCES;
	}

	if (dev->debug > 1) {
		printk("RAW:WRITE:win:%d map:0x%p ofs:0x%X amd:0x%2lx dwd:%d len:%d\n",
		     riob->mapnum, map, riob->offset,
		     mapx->address_modifier, dwidth,
		     riob->bsize);
	}

	for (i = 0, j = riob->offset; i < riob->bsize; i += dwidth, j += dwidth) {
		union vmeio_word *src = (void *)&iob[i];
		if (dwidth == 4)
			iowrite32be(src->width4, &map[j]);
		else if (dwidth == 2)
			iowrite16be(src->width2, &map[j]);
		else
			iowrite8(src->width1, &map[j]);
	}
	kfree(iob);
	return 0;
}

/*
 * =====================================================
 */

int vmeio_ioctl(struct inode *inode, struct file *filp, unsigned int cmd,
		unsigned long arg)
{
	void *arb;		/* Argument buffer area */

	struct vmeio_device *dev;

	int iodr;		/* Io Direction */
	int iosz;		/* Io Size in bytes */
	int cc = 0;		/* erro return code */

	long minor;

	iodr = _IOC_DIR(cmd);
	iosz = _IOC_SIZE(cmd);

	minor = MINOR(inode->i_rdev);
	if (!check_minor(minor))
		return -EACCES;
	dev = &devices[minor];

	if ((arb = kmalloc(iosz, GFP_KERNEL)) == NULL)
		return -ENOMEM;

	if ((iodr & _IOC_WRITE) && copy_from_user(arb, (void *)arg, iosz)) {
		cc = -EACCES;
		goto out;
	}
	debug_ioctl(_IOC_NR(cmd), iodr, iosz, arb, minor, dev->debug);

	if (!dev) {
		cc = -EACCES;
		goto out;
	}

	switch (cmd) {

	case VMEIO_SET_DEBUG:
		vmeio_set_debug(dev, arb);
		break;

	case VMEIO_GET_DEBUG:
		vmeio_get_debug(dev, arb);
		break;

	case VMEIO_GET_VERSION:
		vmeio_get_version(arb);
		break;

	case VMEIO_SET_TIMEOUT:
		vmeio_set_timeout(dev, arb);
		break;

	case VMEIO_GET_TIMEOUT:
		vmeio_get_timeout(dev, arb);
		break;

	case VMEIO_GET_DEVICE:	   /** Get the device described in struct vmeio_get_device_s */
		vmeio_get_device(dev, arb);
		break;

	case VMEIO_SET_DEVICE:     /** Changes the device memory map */
				  /** Super dangerous, experts only */
		vmeio_set_device(dev, arb);
		if (dev->maps[0].vaddr == NULL && dev->maps[1].vaddr == NULL)
			goto out;
		break;

	case VMEIO_RAW_READ_DMA:   /** Raw read VME registers */

		cc = raw_dma(dev, arb, VME_DMA_FROM_DEVICE);
		if (cc < 0)
			goto out;
		break;

	case VMEIO_RAW_WRITE_DMA:  /** Raw write VME registers */

		cc = raw_dma(dev, arb, VME_DMA_TO_DEVICE);
		if (cc < 0)
			goto out;
		break;

	case VMEIO_READ_DMA:   /** Raw read VME registers */
	case VMEIO_WRITE_DMA:  /** Raw write VME registers */

		cc = do_raw_dma(arb);
		if (cc < 0)
			goto out;
		break;

	case VMEIO_RAW_READ:	   /** Raw read VME registers */

		cc = raw_read(dev, arb);
		if (cc < 0)
			goto out;
		break;

	case VMEIO_RAW_WRITE:	   /** Raw write VME registers */
		cc = raw_write(dev, arb);
		if (cc < 0) 
			goto out;
		break;

	default:
		cc = -ENOENT;
		goto out;
		break;
	}

	if ((iodr & _IOC_READ) && copy_to_user((void *)arg, arb, iosz)) {
			cc = -EACCES;
			goto out;
	}
out:	kfree(arb);
	return cc;
}

/* ===================================================== */

static DEFINE_MUTEX(driver_mutex);

/* ===================================================== */

int vmeio_ioctl32(struct inode *inode, struct file *filp, unsigned int cmd,
		  unsigned long arg)
{
	int res;
	mutex_lock(&driver_mutex);
	res = vmeio_ioctl(inode, filp, cmd, arg);
	mutex_unlock(&driver_mutex);
	return res;
}

struct file_operations vmeio_fops = {
	.owner = THIS_MODULE,
	.read = vmeio_read,
	.write = vmeio_write,
	.ioctl = vmeio_ioctl32,
	.open = vmeio_open,
	.release = vmeio_close,
};


module_init(vmeio_install);
module_exit(vmeio_uninstall);

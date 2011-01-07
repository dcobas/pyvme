/**
 * =================================================
 * Implement driver to do basic VME IO
 * Julian Lewis BE/CO/HT Tue 19th October 2010
 *
 * These parameters are vectors that must all be the same length,
 * or simply not used (length = 0). For each lun, the driver may
 * have a level, vector, and at least one VME base address to
 * create the address map. Each window size and address modifier
 * is normally the same for each lun and is specified once or not
 * at all.
 *
 * dname, In order to avoid recompiling the driver it is possible
 *        to define its name via this parameter rather than in
 *        the users parameter file. This means the same kernel
 *        object can be used multiple times.
 *
 * luns, Logical unit numbers (not optional)
 * lvls, Interrupt levels (either not specified or one for each lun)
 * vecs, Interrupt vectors (same as lvls)
 * vme1, First VME base address (not optional)
 * vme2, Second VME base address (either not specified or one for each lun)
 *
 * These parameters would normally be specified once per lun.
 * In this case the driver will use the first value for all luns.
 * There is no restriction however in the driver from specifying
 * them individually for each lun should that be usefull. It may
 * be interesting for example to map one module and use only DMA
 * access for all the others.
 *
 * amd1, First address modifier (not optional, at least one specified)
 * amd2, Second address modifier (optional)
 * dwd1, First data width (not optional, at least one specified)
 * dwd2, Second data width (optional)
 * win1, First window size (not optional, at least one specified)
 * win2, Second window size (optional)
 * nmap, No map window flag, 1=DMA only (defaults to zero)
 * isrc, Location of interrupt source reg in vme1 (optional)
 *
 * Installation via insmod parameters example
 *
 * Example: cp vmeio.ko mytest.ko
 *          insmod mytest.ko dname="mytest"
 *                           luns= 136,   99,    46
 *                           vme1= 0x100, 0x200, 0x300
 *                           amd1= 0x29 win1=0x10 dwd1=2
 *
 * Example: insmod ctrv.ko luns= 1,         2,
 *                         vme1= 0xC100000, 0xC200000
 *                         vme2= 0x100,     0x200
 *                         lvls= 2,         2
 *                         vecs= 0xC8,      0xC9
 *                         amd1= 0x39
 *                         amd2= 0x29
 *                         win1= 0x10000
 *                         win2= 0x100
 *                         dwd1= 4
 *                         dwd2= 2
 *                         isrc= 0
 *
 * ======================================================================
 * Includes
 */

/*
 * These next two includes must be included in this order.
 * Driver configuration definitions.
 * It is supposed that this driver will be compiled under different names
 * and that nodes will be created in /dev for each named kernal module.
 * These definitions must be set up according to a particular useage.
 * DRV_NAME:           is used when printing messages
 * DRV_SYMB:           entry points symbol value
 * DRV_MODULE_VERSION: may change with new features and bug corrections
 * DRV_MAX_DEVICES:    is the number of modules you expect to handle
 */

#include "user_driver_parameters.h"

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
static char *vmeio_major_name = DRV_NAME;

MODULE_AUTHOR("Julian Lewis BE/CO/HT CERN");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Raw IO to VME");
MODULE_SUPPORTED_DEVICE("Any VME device");

/*
 * ==============================
 * Module parameter storage area
 * Indexed by minor device number
 */

static long luns[DRV_MAX_DEVICES];	/* Logical unit numbers */
static long lvls[DRV_MAX_DEVICES];	/* Interrupt levels */
static long vecs[DRV_MAX_DEVICES];	/* Interrupt vectors */
static long vme1[DRV_MAX_DEVICES];	/* First VME base address */
static long vme2[DRV_MAX_DEVICES];	/* Second VME base address */

/* Single value parameter handling */
/* Usually the same for each lun.  */

static long amd1[DRV_MAX_DEVICES];	/* First address modifier */
static long amd2[DRV_MAX_DEVICES];	/* Second address modifier */
static long dwd1[DRV_MAX_DEVICES];	/* First data width */
static long dwd2[DRV_MAX_DEVICES];	/* Second data width */
static long win1[DRV_MAX_DEVICES];	/* First window size */
static long win2[DRV_MAX_DEVICES];	/* Second window size */
static long nmap[DRV_MAX_DEVICES];	/* No map window flag, 1=DMA only */
static long isrc[DRV_MAX_DEVICES];	/* Location of interrupt source reg in vme1 */

/* These parameter counts must equal the number of luns */
/* or be equal to zero if not used. */

static unsigned int luns_num;
static unsigned int lvls_num;
static unsigned int vecs_num;
static unsigned int vme1_num;
static unsigned int vme2_num;

/* These parameter counts are normally zero if not used or */
/* one, they can however take any value between 0..luns */

static unsigned int amd1_num;	/* Normally this value is = "1" */
static unsigned int amd2_num;	/* Can be = "0" if not used */
static unsigned int dwd1_num;	/* Normally this value is = "1" */
static unsigned int dwd2_num;	/* Can be = "0" if not used */
static unsigned int win1_num;	/* Normally this value is = "1" */
static unsigned int win2_num;	/* Can be = "0" if not used */
static unsigned int nmap_num;	/* Its quite possible this is > "1" */
static unsigned int isrc_num;	/* Normally this value is = "1" */

module_param_array(luns, long, &luns_num, 0444);	/* Vector */
module_param_array(lvls, long, &lvls_num, 0444);	/* Vector */
module_param_array(vecs, long, &vecs_num, 0444);	/* Vector */
module_param_array(vme1, long, &vme1_num, 0444);	/* Vector */
module_param_array(vme2, long, &vme2_num, 0444);	/* Vector */

module_param_array(amd1, long, &amd1_num, 0444);
module_param_array(amd2, long, &amd2_num, 0444);
module_param_array(dwd1, long, &dwd1_num, 0444);
module_param_array(dwd2, long, &dwd2_num, 0444);
module_param_array(win1, long, &win1_num, 0444);
module_param_array(win2, long, &win2_num, 0444);
module_param_array(nmap, long, &nmap_num, 0444);
module_param_array(isrc, long, &isrc_num, 0444);

MODULE_PARM_DESC(luns, "Logical unit numbers");
MODULE_PARM_DESC(lvls, "Interrupt levels");
MODULE_PARM_DESC(vecs, "Interrupt vectors");
MODULE_PARM_DESC(vme1, "First map base addresses");
MODULE_PARM_DESC(vme2, "Second map base addresses");

MODULE_PARM_DESC(amd1, "First VME address modifier");
MODULE_PARM_DESC(amd2, "Second VME address modifier");
MODULE_PARM_DESC(dwd1, "First data width 1,2,4,8 bytes");
MODULE_PARM_DESC(dwd2, "Second data width 1,2,4,8 bytes");
MODULE_PARM_DESC(win1, "First window size in bytes");
MODULE_PARM_DESC(win2, "Second window size in bytes");
MODULE_PARM_DESC(nmap, "No VME map flags, 1=DMA only");
MODULE_PARM_DESC(isrc, "Location of interrupt source reg in vme1");

static char dname[64] = { 0 };

module_param_string(dname, dname, sizeof(dname), 0);
MODULE_PARM_DESC(dname, "Driver name");

/*
 * This structure describes all the relevant information about a mapped
 * window
 */
struct vmeio_map {
	unsigned long	base_address;
	unsigned long	address_modifier;
	unsigned long	data_width;
	unsigned long	window_size;
	void		*vaddr;		/* NULL if not mapped */
	struct vme_berr_handler
			*bus_error_handler;	/* NULL if inexistent */
};


/*
 * vmeio device descriptor:
 *	maps[max_maps]		array of mapped VME windows
 *
 *	isrfl			1 if interrupt handler installed
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
	struct vmeio_map	maps[MAX_MAPS];

	int			isrfl;
	void			*isr_source_address;
	int			isr_source_mask;

	wait_queue_head_t	queue;
	int			timeout;
	int			icnt;

	int			debug;
};

/*
 * ==============================================================
 * Mapping/Module context. Its not convenient to use the above
 * module parameter storage area directly. I want a pointer
 * that contains all the data about a modules mappings etc
 */

typedef struct {

	struct vmeio_get_window_s window;

	void *map1;                     /* First mapped vme address */
	void *map2;                     /* Second mapped vme address */
	int isrfl;                      /* Isr installed flag */

	struct vme_berr_handler *ber1;	/* First bus error handler */
	struct vme_berr_handler *ber2;	/* Second bus error handler */

	void *isr_source_address;	/* Where to read in map */
	int isr_source_mask;            /* result of the read */

	wait_queue_head_t queue;	/* For interrupt waiting */
	int timeout;                    /* Timeout value for wait queue */
	int icnt;                       /* Interrupt counter */

	int debug;                      /* Debug level */

} module_context_t;

static module_context_t module_contexts[DRV_MAX_DEVICES];

struct file_operations vmeio_fops;

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

	printk("%s:debug_ioctl:ionr:%d", vmeio_major_name, ionr);
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

/* ================= */

int check_minor(long num)
{
	if (num < 0 || num >= DRV_MAX_DEVICES) {
		printk("%s:minor:%d ", vmeio_major_name, (int) num);
		printk("BAD not in [0..%d]\n", DRV_MAX_DEVICES - 1);
		return 0;
	}
	return 1;
}

/*
 * =========================================================
 * VMEIO with bus error handling
 */

#define CLEAR_BUS_ERROR ((int) 1)
#define BUS_ERR_PRINT_THRESHOLD 10

static int bus_error_count = 0;	/* For all modules */
static int isr_bus_error = 0;	/* Bus error in an ISR */
static int last_bus_error = 0;	/* Last printed bus error */

/* ==================== */

static void BusErrorHandler(struct vme_bus_error *error)
{
	bus_error_count++;
}

/* ==================== */

static int GetClrBusErrCnt(void)
{
	int res;

	res = bus_error_count;

	bus_error_count = 0;
	last_bus_error = 0;
	isr_bus_error = 0;

	return res;
}

/* ==================== */

static void CheckBusError(char *dw, char *dir, void *x)
{
	if (bus_error_count > last_bus_error &&
	    bus_error_count <= BUS_ERR_PRINT_THRESHOLD) {
		printk("%s:BUS_ERROR:%s:%s-Address:0x%p\n",
			       vmeio_major_name, dw, dir, x);
		if (isr_bus_error)
			printk("%s:BUS_ERROR:In ISR occured\n",
				       vmeio_major_name);
		if (bus_error_count == BUS_ERR_PRINT_THRESHOLD)
			printk("%s:BUS_ERROR:PrintSuppressed\n",
				       vmeio_major_name);
		isr_bus_error = 0;
		last_bus_error = bus_error_count;
	}
}

/* ==================== */
/* Used in ISR only     */

static char IHRd8(void *x)
{
	char res;

	isr_bus_error = 0;
	res = ioread8(x);
	if (bus_error_count > last_bus_error)
		isr_bus_error = 1;
	return res;
}

/* ==================== */
/* Used in ISR only     */

static short IHRd16(void *x)
{
	short res;

	isr_bus_error = 0;
	res = ioread16be(x);
	if (bus_error_count > last_bus_error)
		isr_bus_error = 1;
	return res;
}

/* ==================== */
/* Used in ISR only     */

static int IHRd32(void *x)
{
	int res;

	isr_bus_error = 0;
	res = ioread32be(x);
	if (bus_error_count > last_bus_error)
		isr_bus_error = 1;
	return res;
}

/* ==================== */

static int HRd32(void *x)
{
	int res;

	res = ioread32be(x);
	CheckBusError("D32", "READ", x);
	return res;
}

/* ==================== */

static void HWr32(int v, void *x)
{
	iowrite32be(v, x);
	CheckBusError("D32", "WRITE", x);
	return;
}

/* ==================== */

static short HRd16(void *x)
{
	short res;

	res = ioread16be(x);
	CheckBusError("D16", "READ", x);
	return res;
}

/* ==================== */

static void HWr16(short v, void *x)
{
	iowrite16be(v, x);
	CheckBusError("D16", "WRITE", x);
	return;
}

/* ==================== */

static char HRd8(void *x)
{
	char res;

	res = ioread8(x);
	CheckBusError("D8", "READ", x);
	return res;
}

/* ==================== */

static void HWr8(char v, void *x)
{
	iowrite8(v, x);
	CheckBusError("D8", "WRITE", x);
	return;
}

/*
 * =========================================================
 * Interrupt service routine
 * =========================================================
 */

static irqreturn_t vmeio_irq(void *arg)
{
	module_context_t *mcon;
	long data;

	mcon = arg;
	if (mcon->isr_source_address) {
		if (mcon->window.dwd1 == 4)
			data = IHRd32(mcon->isr_source_address);
		else if (mcon->window.dwd1 == 2)
			data = IHRd16(mcon->isr_source_address);
		else
			data = IHRd8(mcon->isr_source_address);
		mcon->isr_source_mask = data;
	}
	mcon->icnt++;
	wake_up(&mcon->queue);
	return IRQ_HANDLED;
}

/* ==================== */

void set_remaining_null(long argarray[], unsigned int argnum)
{
	int i;

	if (!argnum)
		return;
	for (i = argnum; i < luns_num; i++)
		argarray[i] = argarray[0];
}

/* ==================== */

void *map_window(int vme, int win, int amd, int dwd) {

	unsigned long vmeaddr;
	struct pdparam_master param;
	char *msg;

	if (!(vme && amd && win && dwd)) return NULL;

	param.iack = 1;                 /* no iack */
	param.rdpref = 0;               /* no VME read prefetch option */
	param.wrpost = 0;               /* no VME write posting option */
	param.swap = 1;                 /* VME auto swap option */
	param.dum[0] = VME_PG_SHARED;   /* window is sharable */
	param.dum[1] = 0;               /* XPC ADP-type */
	param.dum[2] = 0;               /* window is sharable */

	vmeaddr = find_controller(vme, win, amd, 0, dwd, &param);

	if (vmeaddr == -1UL) {
		msg = "ERROR:NotMapped";
		vmeaddr = 0;
	} else {
		msg = "OK:Mapped";
	}
	printk("%s:%s:Address:0x%X Window:0x%X"
			":AddrMod:0x%X DWidth:0x%X:VirtAddr:0x%lX\n",
			vmeio_major_name, msg,
			vme, win, amd, dwd, vmeaddr);
	return (void *)vmeaddr;
}

/* ==================== */

struct vme_berr_handler *set_berr_handler(int vme, int win, int amd) {

	struct vme_bus_error berr;
	struct vme_berr_handler *handler;

	if (!(vme && amd && win)) return NULL;

	berr.address = (long) vme;
	berr.am      = amd;
	handler      = vme_register_berr_handler(&berr, win, BusErrorHandler);

	printk("%s:BusErrorHandler:", vmeio_major_name);

	if (IS_ERR(handler)) {
	   printk("ERROR:NotRegistered");
	   handler = NULL;
	} else {
	   printk("OK:Registered");
	}
	printk(":Address:0x%X Window:0x%X AddrMod:0x%X\n",vme,win,amd);
	return handler;
}


void register_isr(module_context_t *mcon, unsigned vector, unsigned level)
{
	int err;

	err = vme_intset(vector, (int (*)(void *)) vmeio_irq, mcon, 0);
	mcon->isrfl = !(err < 0);
	printk("%s:ISR:Level:0x%X Vector:0x%X:%s\n",
		vmeio_major_name, level, vector,
		(err < 0) ? "ERROR:NotRegistered" : "OK:Registered");
}

void register_int_source(module_context_t *mcon, void *map, unsigned offset)
{
	mcon->isr_source_address = mcon->map1 + mcon->window.isrc;
	printk("SourceRegister:0x%p", mcon->isr_source_address);
}

/*
 * =====================================================
 * Install
 * =====================================================
 */

int vmeio_install(void)
{
	int i, cc;
	module_context_t *mcon;

	if (luns_num <= 0 || luns_num > DRV_MAX_DEVICES) {
		printk("%s:Fatal:No logical units defined.\n",
		       vmeio_major_name);
		return -EACCES;
	}

	/* Vector parameters must all be the same size or zero */

	if (lvls_num != luns_num && lvls_num != 0) {
		printk("%s:Fatal:Missing interrupt level.\n",
		       vmeio_major_name);
		return -EACCES;
	}

	if (vecs_num != luns_num && vecs_num != 0) {
		printk("%s:Fatal:Missing interrupt vector.\n",
		       vmeio_major_name);
		return -EACCES;
	}

	if (vme1_num != luns_num && vme1_num != 0) {
		printk("%s:Fatal:Missing first base address.\n",
		       vmeio_major_name);
		return -EACCES;
	}
	if (vme2_num != luns_num && vme2_num != 0) {
		printk("%s:Fatal:Missing second base address.\n",
		       vmeio_major_name);
		return -EACCES;
	}

	/* Default single parameters to the first specified */

	set_remaining_null(amd1, amd1_num);
	set_remaining_null(amd2, amd2_num);
	set_remaining_null(dwd1, dwd1_num);
	set_remaining_null(dwd2, dwd2_num);
	set_remaining_null(win1, win1_num);
	set_remaining_null(win2, win2_num);
	set_remaining_null(nmap, nmap_num);
	set_remaining_null(isrc, isrc_num);
	set_remaining_null(amd2, amd2_num);

	/* Build module contexts */

	for (i = 0; i < luns_num; i++) {

		mcon = &module_contexts[i];
		memset(mcon, 0, sizeof(module_context_t));

		mcon->window.lun = luns[i];
		mcon->window.lvl = lvls[i];
		mcon->window.vec = vecs[i];
		mcon->window.nmap = nmap[i];
		mcon->window.amd1 = amd1[i];
		mcon->window.amd2 = amd2[i];
		mcon->window.dwd1 = dwd1[i];
		mcon->window.dwd2 = dwd2[i];
		mcon->window.win1 = win1[i];
		mcon->window.win2 = win2[i];
		mcon->window.nmap = nmap[i];
		mcon->window.isrc = isrc[i];
		mcon->window.vme1 = vme1[i];
		mcon->window.vme2 = vme2[i];

		init_waitqueue_head(&mcon->queue);
	}

	/* Register driver */

	cc = register_chrdev(vmeio_major, vmeio_major_name, &vmeio_fops);
	if (cc < 0) {
		printk("%s:Fatal:Error from register_chrdev [%d]\n",
		       vmeio_major_name, cc);
		return cc;
	}
	if (vmeio_major == 0)
		vmeio_major = cc;	/* dynamic */

	/* Create VME mappings and register ISRs */

	for (i = 0; i < luns_num; i++) {

		mcon = &module_contexts[i];
		mcon->debug = DEBUG;
		mcon->timeout = msecs_to_jiffies(TIMEOUT);
		mcon->icnt = 0;

		if (strlen(dname))
			vmeio_major_name = dname;

		if (!mcon->window.nmap) {

			printk("\n%s:Mapping:Logical unit:%d\n",
			     vmeio_major_name, mcon->window.lun);

			mcon->map1 = map_window(mcon->window.vme1,mcon->window.win1,mcon->window.amd1,mcon->window.dwd1);
			mcon->ber1 = set_berr_handler(mcon->window.vme1,mcon->window.win1,mcon->window.amd1);

			mcon->map2 = map_window(mcon->window.vme2,mcon->window.win2,mcon->window.amd2,mcon->window.dwd2);
			mcon->ber2 = set_berr_handler(mcon->window.vme2,mcon->window.win2,mcon->window.amd2);

			if (mcon->window.lvl && mcon->window.vec) {
				register_isr(mcon, mcon->window.vec, mcon->window.lvl);
				if (isrc_num)
					register_int_source(mcon, mcon->map1,  mcon->window.isrc);
			}

		} else {
			printk("%s:Logical unit:%d is not mapped: DMA only\n",
			     vmeio_major_name, mcon->window.lun);
		}
	}
	return 0;
}

/* ==================== */

void unregister_module(module_context_t *mcon) {

	if (mcon->window.vec)
		vme_intclr(mcon->window.vec, NULL);
	if (mcon->map1)
		return_controller((unsigned) mcon->map1,
				  mcon->window.win1);
	if (mcon->map2)
		return_controller((unsigned) mcon->map2,
				  mcon->window.win2);
	if (mcon->ber1)
		vme_unregister_berr_handler(mcon->ber1);
	if (mcon->ber2)
		vme_unregister_berr_handler(mcon->ber2);
}

/*
 * =====================================================
 * Uninstall the driver
 * =====================================================
 */

void vmeio_uninstall(void)
{
	int i;
	module_context_t *mcon;

	for (i = 0; i < luns_num; i++) {
		mcon = &module_contexts[i];
		unregister_module(mcon);
	}
	unregister_chrdev(vmeio_major, vmeio_major_name);
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
	long num;
	struct inode *inode;

	struct vmeio_read_buf_s rbuf;
	module_context_t *mcon;
	int icnt;

	inode = filp->f_dentry->d_inode;
	num = MINOR(inode->i_rdev);
	if (!check_minor(num))
		return -EACCES;
	mcon = &module_contexts[num];

	if (mcon->debug) {
		printk("%s:read:count:%d minor:%d\n", vmeio_major_name,
		       count, (int) num);
		if (mcon->debug > 1) {
			printk("%s:read:timout:%d\n", vmeio_major_name,
			       mcon->timeout);
		}
	}

	if (count < sizeof(struct vmeio_read_buf_s)) {
		if (mcon->debug) {
			printk("%s:read:Access error buffer too small\n",
			       vmeio_major_name);
		}
		return -EACCES;
	}

	icnt = mcon->icnt;
	if (mcon->timeout) {
		cc = wait_event_interruptible_timeout(mcon->queue,
						      icnt != mcon->icnt,
						      mcon->timeout);
	} else {
		cc = wait_event_interruptible(mcon->queue,
					      icnt != mcon->icnt);
	}

	if (mcon->debug > 2) {
		printk("%s:wait_event:returned:%d\n", vmeio_major_name,
		       cc);
	}

	if (cc == -ERESTARTSYS) {
		printk("%s:vmeio_read:interrupted by signal\n",
		       vmeio_major_name);
		return cc;
	}
	if (cc == 0 && mcon->timeout)
		return -ETIME;	/* Timer expired */
	if (cc < 0)
		return cc;	/* Error */

	rbuf.logical_unit = mcon->window.lun;
	rbuf.interrupt_mask = mcon->isr_source_mask;
	rbuf.interrupt_count = mcon->icnt;

	cc = copy_to_user(buf, &rbuf, sizeof(struct vmeio_read_buf_s));
	if (cc != 0) {
		printk("%s:Can't copy to user space:cc=%d\n", vmeio_major_name, cc);
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
	long num;
	module_context_t *mcon;
	struct inode *inode;
	int cc, mask;

	inode = filp->f_dentry->d_inode;
	num = MINOR(inode->i_rdev);
	if (!check_minor(num))
		return -EACCES;
	mcon = &module_contexts[num];

	if (count < sizeof(int)) {
		cc = copy_from_user(&mask, buf, sizeof(int));
		if (cc != 0) {
			printk("%s:write:Error:%d could not copy from user\n",
					     vmeio_major_name, cc);
			return -EACCES;
		}
	}

	if (mcon->debug) {
		printk("%s:write:count:%d minor:%d mask:0x%X\n",
		       vmeio_major_name, count, (int) num, mask);
	}

	mcon->isr_source_mask = mask;
	mcon->icnt++;
	wake_up(&mcon->queue);
	return sizeof(int);
}

/*
 * =====================================================
 * Ioctl
 * =====================================================
 */

static inline int ioctl_err(int er, void *p, void *q)
{
	if (p) kfree(p);
	if (q) kfree(q);
	return er;
}

#define VME_NO_ADDR_INCREMENT 1
#define DMA_BLOCK_SIZE        4096
#define SAMPLES_IN_DMA_BLOCK  2048

static void vmeio_set_debug(module_context_t *mcon, int *debug)
{
	mcon->debug = *debug;
}

static void vmeio_get_debug(module_context_t *mcon, int *debug)
{
	*debug = mcon->debug;
}

static void vmeio_get_version(int *version)
{
	*version = COMPILE_TIME;
}

static void vmeio_set_timeout(module_context_t *mcon, int *timeout)
{
	mcon->timeout = msecs_to_jiffies(*timeout);
}

static void vmeio_get_timeout(module_context_t *mcon, int *timeout)
{
	*timeout = jiffies_to_msecs(mcon->timeout);
}

static void vmeio_get_device(module_context_t *mcon, 
		struct vmeio_get_window_s *win)
{
		memcpy(win, &mcon->window, sizeof(*win));
}

static void vmeio_set_map(module_context_t *mcon, 
			struct vmeio_map *map, 
			unsigned mapno)
{
	void *vaddr;

	vaddr = map_window(map->base_address, map->window_size,
			map->address_modifier, map->data_width);
}

/*
 * =====================================================
 */

int vmeio_ioctl(struct inode *inode, struct file *filp, unsigned int cmd,
		unsigned long arg)
{
	void *arb;		/* Argument buffer area */
	char *map, *iob;	/* Io memory hardware map pointer and local buffer */

	module_context_t *mcon;

	struct vmeio_riob_s *riob;
	struct vmeio_get_window_s *winb;

	struct vme_dma dma_desc;

	int iodr;		/* Io Direction */
	int iosz;		/* Io Size in bytes */

	int *ibp, *imp;		/* Integer pointers used to transfer between buf and map */
	short *sbp, *smp;	/* Short transfer pointers */
	char *cbp, *cmp;	/* And for chars */

	int i, j, cnt, cc, amd, dwd, win;

	unsigned int bu, bl;	/* Upper and lower 32 bits of buffer address */

	long num;

	iodr = _IOC_DIR(cmd);
	iosz = _IOC_SIZE(cmd);

	num = MINOR(inode->i_rdev);
	if (!check_minor(num))
		return -EACCES;
	mcon = &module_contexts[num];

	if ((arb = kmalloc(iosz, GFP_KERNEL)) == NULL)
		return -ENOMEM;

	if (iodr & _IOC_WRITE) {
		if (copy_from_user(arb, (void *)arg, iosz) != 0)
			ioctl_err(-EACCES, arb, NULL);
	}
	debug_ioctl(_IOC_NR(cmd), iodr, iosz, arb, num, mcon->debug);

	if (!mcon)
		return ioctl_err(-EACCES, arb, NULL);

	switch (cmd) {

	case VMEIO_SET_DEBUG:
		vmeio_set_debug(mcon, arb);
		break;

	case VMEIO_GET_DEBUG:
		vmeio_get_debug(mcon, arb);
		break;

	case VMEIO_GET_VERSION:
		vmeio_get_version(arb);
		break;

	case VMEIO_SET_TIMEOUT:
		vmeio_set_timeout(mcon, arb);
		break;

	case VMEIO_GET_TIMEOUT:
		vmeio_get_timeout(mcon, arb);
		break;

	case VMEIO_GET_DEVICE:	   /** Get the device described in struct vmeio_get_device_s */
		vmeio_get_device(mcon, arb);
		break;

	case VMEIO_SET_DEVICE:     /** Changes the device memory map */
				  /** Super dangerous, experts only */

		winb = arb;
		if (!mcon->window.nmap) {

			unregister_module(mcon);

			/** Map new hardware window */

			memcpy(&mcon->window, winb, sizeof(struct vmeio_get_window_s));

			mcon->map1 = map_window(mcon->window.vme1,mcon->window.win1,mcon->window.amd1,mcon->window.dwd1);
			mcon->ber1 = set_berr_handler(mcon->window.vme1,mcon->window.win1,mcon->window.amd1);
			mcon->map2 = map_window(mcon->window.vme2,mcon->window.win2,mcon->window.amd2,mcon->window.dwd2);
			mcon->ber2 = set_berr_handler(mcon->window.vme2,mcon->window.win2,mcon->window.amd2);

			if ((mcon->map1 == NULL) && (mcon->map2 == NULL)) {                 /** Not good ! */
				printk("%s:ERROR:No mapped hardware, DMA only\n",
				       vmeio_major_name);
				return ioctl_err(-EIO, arb, NULL);                          /** Too late */
			}
		} else
			memcpy(&mcon->window, winb, sizeof(struct vmeio_get_window_s));     /** DMA only */
		break;

	case VMEIO_RAW_READ_DMA:   /** Raw read VME registers */

		riob = arb;

#ifdef __64BIT
		bl = (unsigned int)riob->buffer & 0xFFFFFFFF;
		bu = (unsigned int)riob->buffer >> 32;
#else
		bl = (unsigned int)riob->buffer;
		bu = 0;
#endif
		memset(&dma_desc, 0,sizeof(struct vme_dma));

		if (riob->winum == 2) {	/* Second window */
			map = (char *) mcon->window.vme2;
			amd = mcon->window.amd2;
			dwd = mcon->window.dwd2;
		} else {	/* First window */
			map = (char *) mcon->window.vme1;
			amd = mcon->window.amd1;
			dwd = mcon->window.dwd1;
		}

		map += riob->offset;

		dma_desc.dir = VME_DMA_FROM_DEVICE;
		dma_desc.src.data_width = dwd * 8;
		dma_desc.src.am = amd;
		dma_desc.novmeinc = 0;

		dma_desc.ctrl.pci_block_size = VME_DMA_BSIZE_4096;
		dma_desc.ctrl.pci_backoff_time = VME_DMA_BACKOFF_0;
		dma_desc.ctrl.vme_block_size = VME_DMA_BSIZE_4096;
		dma_desc.ctrl.vme_backoff_time = VME_DMA_BACKOFF_0;

		dma_desc.src.addrl = (unsigned int) map;
		dma_desc.dst.addrl = bl;
		dma_desc.dst.addru = bu;
		dma_desc.length = riob->bsize;

		if (mcon->debug > 1) {
			printk
			    ("DMA:READ:win:%d src:0x%p amd:0x%x dwd:%d len:%d dst:0x%08x%08x\n",
			     riob->winum, map, amd, dwd, riob->bsize, bu,
			     bl);
		}

		cc = vme_do_dma(&dma_desc);
		if (cc < 0)
			return (ioctl_err(cc, arb, NULL));

		if (!(dma_desc.status & TSI148_LCSR_DSTA_DON)) {
			printk("%s:DMA:NotDone:Status:0x%X\n",
			       vmeio_major_name, dma_desc.status);
			return ioctl_err(-EIO, arb, NULL);
		}
		break;

	case VMEIO_RAW_WRITE_DMA:  /** Raw write VME registers */

		riob = arb;

#ifdef __64BIT
		bl = (unsigned int)riob->buffer & 0xFFFFFFFF;
		bu = (unsigned int)riob->buffer >> 32;
#else
		bl = (unsigned int)riob->buffer;
		bu = 0;
#endif
		memset(&dma_desc, 0, sizeof(struct vme_dma));

		if (riob->winum == 2) {	/* Second window */
			map = (char *) mcon->window.vme2;
			amd = mcon->window.amd2;
			dwd = mcon->window.dwd2;
		} else {	/* First window */
			map = (char *) mcon->window.vme1;
			amd = mcon->window.amd1;
			dwd = mcon->window.dwd1;
		}

		map += riob->offset;

		dma_desc.dir = VME_DMA_TO_DEVICE;
		dma_desc.dst.data_width = dwd * 8;
		dma_desc.dst.am = amd;
		dma_desc.novmeinc = 0;

		dma_desc.ctrl.pci_block_size = VME_DMA_BSIZE_4096;
		dma_desc.ctrl.pci_backoff_time = VME_DMA_BACKOFF_0;
		dma_desc.ctrl.vme_block_size = VME_DMA_BSIZE_4096;
		dma_desc.ctrl.vme_backoff_time = VME_DMA_BACKOFF_0;

		dma_desc.src.addrl = bl;
		dma_desc.src.addru = bu;
		dma_desc.dst.addrl = (unsigned int) map;
		dma_desc.length = riob->bsize;

		if (mcon->debug > 1) {
			printk
			    ("DMA:WRITE:win:%d dst:0x%p amd:0x%x dwd:%d len:%d src:0x%08x%08x\n",
			     riob->winum, map, amd, dwd, riob->bsize, bu,
			     bl);
		}

		cc = vme_do_dma(&dma_desc);
		if (cc < 0)
			return (ioctl_err(cc, arb, NULL));

		if (!(dma_desc.status & TSI148_LCSR_DSTA_DON)) {
			printk("%s:DMA:NotDone:Status:0x%X\n",
			       vmeio_major_name, dma_desc.status);
			return ioctl_err(-EIO, arb, NULL);
		}
		break;

	case VMEIO_RAW_READ:	   /** Raw read VME registers */

		riob = arb;

		if (mcon->window.nmap)
			return ioctl_err(-ENODEV, arb, NULL); /* Not mapped */

		if (riob->bsize > vmeioMAX_BUF)
			return -E2BIG;
		iob = kmalloc(riob->bsize, GFP_KERNEL);
		if (!iob)
			return ioctl_err(-ENOMEM, arb, NULL);

		if (riob->winum == 2) {	/* Second window */
			map = mcon->map2;
			if (!map)
				return ioctl_err(-ENODEV, arb, iob);
			dwd = mcon->window.dwd2;
			amd = mcon->window.amd2;
			win = mcon->window.win2;
		} else {	/* First window */
			map = mcon->map1;
			if (!map)
				return ioctl_err(-ENODEV, arb, iob);
			dwd = mcon->window.dwd1;
			amd = mcon->window.amd1;
			win = mcon->window.win1;
		}

		if (mcon->debug > 1) {
			printk
			    ("RAW:READ:win:%d map:0x%p offs:0x%X amd:0x%x dwd:%d len:%d\n",
			     riob->winum, map, riob->offset, amd, dwd,
			     riob->bsize);
		}

		cnt = GetClrBusErrCnt();
		for (i = 0, j = riob->offset; i < riob->bsize;
		     i += dwd, j += dwd) {
			if (dwd == 4) {
				ibp = (int *) &iob[i];		/* 32 bit access */
				imp = (int *) &map[j];
				*ibp = HRd32(imp);
			} else if (dwd == 2) {
				sbp = (short *) &iob[i];	/* 16 bit access */
				smp = (short *) &map[j];
				*sbp = HRd16(smp);
			} else {
				cbp = &iob[i];	/* 8 bit access */
				cmp = &map[j];
				*cbp = HRd8(cmp);
			}
			if (GetClrBusErrCnt())
				return ioctl_err(-EIO, arb, iob);
		}
		cc = copy_to_user(riob->buffer, iob, riob->bsize);
		kfree(iob);
		if (cc)
			return ioctl_err(-EACCES, arb, NULL);
		break;

	case VMEIO_RAW_WRITE:	   /** Raw write VME registers */
		riob = arb;

		if (mcon->window.nmap)
			return ioctl_err(-ENODEV, arb, NULL); /* Not mapped */

		if (riob->bsize > vmeioMAX_BUF)
			return ioctl_err(-E2BIG, arb, NULL);
		iob = kmalloc(riob->bsize, GFP_KERNEL);
		if (!iob)
			return ioctl_err(-ENOMEM, arb, NULL);


		cc = copy_from_user(iob, riob->buffer, riob->bsize);

		if (riob->winum == 2) {	/* Second window */
			map = mcon->map2;
			if (!map)
				return ioctl_err(-ENODEV, arb, iob);
			dwd = mcon->window.dwd2;
			amd = mcon->window.amd2;
			win = mcon->window.win2;
		} else {	/* First window */
			map = mcon->map1;
			if (!map)
				return ioctl_err(-ENODEV, arb, iob);
			dwd = mcon->window.dwd1;
			amd = mcon->window.amd1;
			win = mcon->window.win1;
		}

		if (mcon->debug > 1) {
			printk
			    ("RAW:WRITE:win:%d map:0x%p ofs:0x%X amd:0x%x dwd:%d len:%d\n",
			     riob->winum, map, riob->offset, amd, dwd,
			     riob->bsize);
		}

		cnt = GetClrBusErrCnt();
		for (i = 0, j = riob->offset; i < riob->bsize;
		     i += dwd, j += dwd) {
			if (dwd == 4) {
				ibp = (int *) &iob[i];	/* 32 bit access */
				imp = (int *) &map[j];
				HWr32(*ibp, imp);
			} else if (dwd == 2) {
				sbp = (short *) &iob[i];	/* 16 bit access */
				smp = (short *) &map[j];
				HWr16(*sbp, smp);
			} else {
				cbp = &iob[i];	/* 8 bit access */
				cmp = &map[j];
				HWr8(*cbp, cmp);
			}
			if (GetClrBusErrCnt())
				return ioctl_err(-EIO, arb, iob);
		}
		kfree(iob);
		break;

	default:
		return ioctl_err(-ENOENT, arb, NULL);
	}

	if (iodr & _IOC_READ) {
		if (copy_to_user((void *)arg, arb, iosz) != 0)
			return ioctl_err(-EACCES, arb, NULL);
	}
	return ioctl_err(0, arb, NULL);
}

/* ===================================================== */

static DEFINE_MUTEX(driver_mutex);

/* ===================================================== */

long vmeio_ioctl64(struct file *filp, unsigned int cmd, unsigned long arg)
{
	int res;
	mutex_lock(&driver_mutex);
	res = vmeio_ioctl(filp->f_dentry->d_inode, filp, cmd, arg);
	mutex_unlock(&driver_mutex);
	return res;
}

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
	.read = vmeio_read,
	.write = vmeio_write,
	.ioctl = vmeio_ioctl32,
	.compat_ioctl = vmeio_ioctl64,
	.open = vmeio_open,
	.release = vmeio_close,
};


module_init(vmeio_install);
module_exit(vmeio_uninstall);

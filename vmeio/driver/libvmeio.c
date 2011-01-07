#include <linux/ioctl.h>
#include <fcntl.h>
#include <stdio.h>
#include "vmeio.h"

void print_riob(struct vmeio_riob_s *p)
{
	printf("IOCTL: win %d at offset 0x%08x width %d\n", 
		p->winum, p->offset, p->bsize);
}

int vmeio_open(int lun)
{
	static char devname[128];

	snprintf(devname, 128, "/dev/vmeio.%d", lun);
	return open(devname, O_RDWR);
}

int vmeio_dma_read(int fd, struct vmeio_riob_s *riobp)
{
	return ioctl(fd, VMEIO_RAW_READ_DMA, riobp);
}

int vmeio_dma_write(int fd, struct vmeio_riob_s *riobp)
{
	return ioctl(fd, VMEIO_RAW_WRITE_DMA, riobp);
}

int vmeio_raw_read(int fd, struct vmeio_riob_s *riobp)
{
	return ioctl(fd, VMEIO_RAW_READ, riobp);
}
int vmeio_raw_write(int fd, struct vmeio_riob_s *riobp)
{
	return ioctl(fd, VMEIO_RAW_WRITE, riobp);
}

int vmeio_set_device(int fd, struct vmeio_get_window_s *getwinp)
{
	printf("setting device (%x, %x, %x, %x)\n", 
		getwinp->vme1,
		getwinp->amd1,
		getwinp->dwd1,
		getwinp->win1);
	return ioctl(fd, VMEIO_SET_DEVICE, getwinp);
}

int vmeio_set_timeout(int fd, int msecs)
{
	return ioctl(fd, VMEIO_SET_TIMEOUT, &msecs);
}

int vmeio_wait_irq(int fd, int msecs_timeout)
{
	struct vmeio_read_buf_s rb;
	int ret;

	ret = ioctl(fd, VMEIO_SET_TIMEOUT, &msecs_timeout);
	printf("set timeout returned %d\n", ret);
	ret = read(fd, &rb, sizeof rb);
	printf("ret = %x, int count = %x, lun = %d",
		ret, rb.interrupt_count, rb.logical_unit);
	return rb.interrupt_count;
}

int vmeio_close(int fd)
{
	return close(fd);
}

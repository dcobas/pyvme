#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "vmeio.h"
#include "libencore.h"

#define VME_DMA_DEV	"/dev/vme_dma"
#define MAX_FILENAME	256
static char devtemplate[] = "/dev/%s.%d";

encore_handle encore_open(char *devname, int lun)
{
	char	tmp[MAX_FILENAME];
	int	cc;
	encore_handle	ret;

	if ((ret = malloc(sizeof(*ret))) == NULL) {
		errno = ENOMEM;
		return NULL;
	}
	cc = snprintf(tmp, MAX_FILENAME, devtemplate, devname, lun);
	if (cc < 0 || cc >= MAX_FILENAME) {
		errno = EINVAL;
		goto fail;
	}
	if ((ret->fd = open(tmp, O_RDWR)) < 0) {
		errno = EINVAL;
		goto fail;
	}

	if (ioctl(ret->fd, VMEIO_GET_NREGS, &ret->nregs) < 0) {
		errno = ENODEV;
		goto fail2;
	}

	ret->reginfo = malloc(ret->nregs*sizeof(struct encore_reginfo));
	if (ret->reginfo == NULL) {
		errno = ENOMEM;
		goto fail2;
	}
	if (ioctl(ret->fd, VMEIO_GET_REGINFO, &ret->reginfo) < 0) {
		errno = ENODEV;
		goto fail3;
	}
	if ((ret->dmafd = open(VME_DMA_DEV, O_RDWR)) < 0) {
		errno = ENODEV;
		goto fail3;
	}

	return ret;

fail3:	free(ret->reginfo);
fail2:	close(ret->fd);
fail:	free(ret);
	return NULL;
}

int encore_close(encore_handle h)
{
	int ret = 0;

	if (close(h->dmafd) < 0 || close(h->fd) < 0)
		ret = -1;
	free(h->reginfo);
	free(h);
	return ret;
}

int encore_set_timeout(encore_handle h, int timeout)
{
	return ioctl(h->fd, VMEIO_SET_TIMEOUT, &timeout);
}

int encore_get_timeout(encore_handle h, int *timeout)
{
	return ioctl(h->fd, VMEIO_GET_TIMEOUT, timeout);
}

int encore_wait(encore_handle h)
{
	struct vmeio_read_buf_s rb;
	return read(h->fd, &rb, sizeof(rb));
}

int encore_reg_id(encore_handle h, char *regname)
{
	int i;

	for (i = 0; i < h->nregs; i++) {
		if (strcmp(regname, h->reginfo[i].name) == 0)
			return i;
	}
	return -1;
}

int encore_raw_read(encore_handle h, int map,
	unsigned offset, int items, int data_width, void *dst)
{
	struct vmeio_riob riob;

	riob.mapnum = map;
	riob.offset = offset;
	riob.wsize  = items;
	riob.data_width = data_width;
	riob.buffer = dst;
	return ioctl(h->fd, VMEIO_RAW_READ, &riob);
}

int encore_raw_write(encore_handle h, int map,
	unsigned offset, int items, int data_width, void *src)
{
	struct vmeio_riob riob;

	riob.mapnum = map;
	riob.offset = offset;
	riob.wsize  = items;
	riob.data_width = data_width;
	riob.buffer = src;
	return ioctl(h->fd, VMEIO_RAW_WRITE, &riob);
}

int encore_get_window(encore_handle h, int reg_id, int from, int to,
					void *dst)
{
	struct encore_reginfo *reg;
	unsigned offset;

	if (reg_id < 0 || reg_id >= h->nregs)
		return -1;
	reg = &h->reginfo[reg_id];
	offset = reg->offset + to * (reg->data_width/8);
	return encore_raw_read(h, reg->block_address_space,
		offset, to-from, reg->data_width, dst);
}

int encore_set_window(encore_handle h, int reg_id, int from, int to,
					void *src)
{
	struct encore_reginfo *reg;
	unsigned offset;

	if (reg_id < 0 || reg_id >= h->nregs)
		return -1;
	reg = &h->reginfo[reg_id];
	offset = reg->offset + to * (reg->data_width/8);
	return encore_raw_write(h, reg->block_address_space,
		offset, to-from, reg->data_width, src);
}

int encore_get_register(encore_handle h, int reg_id, unsigned int *value)
{
	return encore_get_window(h, reg_id, 0, 1, value);
}

int encore_set_register(encore_handle h, int reg_id, unsigned int value)
{
	return encore_set_window(h, reg_id, 0, 1, &value);
}

int encore_dma_read(encore_handle h, unsigned long address,
	unsigned am, unsigned data_width, unsigned long size,
	void *dst)
{
	struct vme_dma dma_desc;

	dma_desc.dir = VME_DMA_FROM_DEVICE;

	dma_desc.src.data_width = data_width;
	dma_desc.src.am = am;
	dma_desc.src.addru = 0;
	dma_desc.src.addrl = address;
	dma_desc.dst.addru = 0;
	dma_desc.dst.addrl = (unsigned int) dst;
	dma_desc.length = size;

	dma_desc.ctrl.pci_block_size = VME_DMA_BSIZE_4096;
	dma_desc.ctrl.pci_backoff_time = VME_DMA_BACKOFF_0;
	dma_desc.ctrl.vme_block_size = VME_DMA_BSIZE_4096;
	dma_desc.ctrl.vme_backoff_time = VME_DMA_BACKOFF_0;

	return ioctl(h->dmafd, VME_IOCTL_START_DMA, &dma_desc);
}

int encore_dma_write(encore_handle h, unsigned long address,
	unsigned am, unsigned data_width, unsigned long size,
	void *src)
{
	struct vme_dma dma_desc;

	dma_desc.dir = VME_DMA_TO_DEVICE;

	dma_desc.dst.data_width = data_width;
	dma_desc.dst.am = am;
	dma_desc.dst.addru = 0;
	dma_desc.dst.addrl = address;
	dma_desc.src.addru = 0;
	dma_desc.src.addrl = (unsigned int) src;
	dma_desc.length = size;

	dma_desc.ctrl.pci_block_size = VME_DMA_BSIZE_4096;
	dma_desc.ctrl.pci_backoff_time = VME_DMA_BACKOFF_0;
	dma_desc.ctrl.vme_block_size = VME_DMA_BSIZE_4096;
	dma_desc.ctrl.vme_backoff_time = VME_DMA_BACKOFF_0;

	return ioctl(h->dmafd, VME_IOCTL_START_DMA, &dma_desc);
}

int encore_dma_get_register(encore_handle h, int reg_id, unsigned int *value)
{
	return -1;
}

int encore_dma_set_register(encore_handle h,
			int reg_id, unsigned int value)
{
	return -1;
}

int encore_dma_get_window(encore_handle h, int reg_id, int from, int to,
					void *dst)
{
	return -1;
}

int encore_dma_set_window(encore_handle h, int reg_id, int from, int to,
					void *src)
{
	return -1;
}

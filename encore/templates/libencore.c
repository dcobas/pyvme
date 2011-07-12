#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "vmeio.h"

#define MAX_FILENAME	256

typedef struct encore_handle {
	int			fd;
	int			nregs;
	struct encore_reginfo	*reginfo;
} *encore_handle;

static char devtemplate[] = "/dev/%s.%d";

encore_handle encore_open(char *devname, int lun)
{
	char	tmp[MAX_FILENAME];
	int	nregs;
	int	cc;
	struct encore_handle	*ret;

	if ((ret = malloc(sizeof(*ret))) == NULL) {
		errno = -ENOMEM;
		return NULL;
	}
	cc = snprintf(tmp, MAX_FILENAME, devtemplate, devname, lun);
	if (cc < 0 || cc >= MAX_FILENAME) {
		errno = -EINVAL;
		return NULL;
	}
	ret->fd = open(tmp, O_RDWR);

	if ((cc = ioctl(ret->fd, VMEIO_GET_NREGS, &ret->nregs)) < 0) {
		errno = -ENODEV;
		return NULL;
	}

	ret->reginfo = malloc(nregs*sizeof(struct encore_reginfo));
	if (ret->reginfo == NULL) {
		errno = -ENOMEM;
		return NULL;
	}
	if ((cc = ioctl(ret->fd, VMEIO_GET_REGINFO, &ret->reginfo)) < 0) {
		errno = -ENODEV;
		return NULL;
	}

	return ret;
}

int encore_close(encore_handle h)
{
	int fd = h->fd;

	free(h->reginfo);
	free(h);
	return close(fd);
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

int encore_read(encore_handle h, int reg_id, unsigned int *value)
{
	struct vmeio_riob riob;
	struct encore_reginfo *reg;

	if (reg_id < 0 || reg_id >= h->nregs)
		return -1;

	reg = &h->reginfo[reg_id];
	riob.mapnum	= reg->block_address_space;
	riob.offset	= reg->offset;
	riob.wsize	= 0;
	riob.buffer	= value;
	riob.data_width	= reg->data_width;
	return ioctl(h->fd, VMEIO_RAW_READ, &riob);
}

int encore_write(encore_handle h, int reg_id, int value)
{
	struct vmeio_riob riob;
	struct encore_reginfo *reg;

	if (reg_id < 0 || reg_id >= h->nregs)
		return -1;

	reg = &h->reginfo[reg_id];
	riob.mapnum	= reg->block_address_space;
	riob.offset	= reg->offset;
	riob.wsize	= 0;
	riob.buffer	= &value;
	riob.data_width	= reg->data_width;
	return ioctl(h->fd, VMEIO_RAW_WRITE, &riob);
}

int encore_dma(encore_handle h, struct vmeio_dma_op *dmaop)
{
	return 0;
}

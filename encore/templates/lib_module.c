#include <sys/ioctl.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include "vmeio.h"
#include "${driver_name}_regs.h"

#define MAX_DEVNAME	256

int ${driver_name}_open(int lun)
{
	char devname[MAX_DEVNAME];
	static char devnamefmt[] = "/dev/${driver_name}.%d";
	int cc;

	cc = snprintf(devname, MAX_DEVNAME, devnamefmt, lun);
	if (cc < 0 || cc >= MAX_DEVNAME)
		return -EINVAL;
	return open(devname, O_RDWR);
}

int ${driver_name}_close(int fd)
{
	return close(fd);
}

int ${driver_name}_wait(int fd)
{
	struct vmeio_read_buf_s rb;

	return read(fd, &rb, sizeof(rb));
}

enum encore_direction {
	ENCORE_READ = 0,
	ENCORE_WRITE = 1,
};

int ${driver_name}_raw(int fd, int mapping,
	unsigned offset, unsigned items, unsigned data_width,
	void *buffer, enum encore_direction write)
{
	struct vmeio_riob riob, *riobp = &riob;

	riobp->mapnum = mapping;
	riobp->offset = offset;
	riobp->wsize  = items;
	riobp->buffer = buffer;
	riobp->data_width = data_width;

	if (write)
		return ioctl(fd, VMEIO_RAW_WRITE, riobp);
	else
		return ioctl(fd, VMEIO_RAW_READ, riobp);
}

static int reg_wnum(struct encore_reginfo *reg)
{
	char sz = reg->wordsize[0];

	switch (sz) {
	case 'c':
		return 8;
	case 's':
		return 16;
	case 'l':
		return 32;
	default:
		return 0;
	}
}

static int get_set_register(int fd,
	struct encore_reginfo *reg, void *buffer,
	enum encore_direction direction)
{
	int data_width = reg_wnum(reg);

	return ${driver_name}_raw(fd,
		reg->block_address_space,
		reg->offset, reg->depth,
		data_width, buffer, direction);
}

static int get_set_window(int fd,
	struct encore_reginfo *reg,
	void *buffer, int from, int to,
	enum encore_direction direction)
{
	int data_width = reg_wnum(reg);
	int bytesize = data_width/8;

	return ${driver_name}_raw(fd,
		reg->block_address_space,
		reg->offset + from*bytesize,
		to - from,
		data_width, buffer, direction);
}

${get_set_routines}


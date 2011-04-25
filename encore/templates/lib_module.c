#include <sys/ioctl.h>
#include <errno.h>
#include "vmeio.h"
#include "%(driver_name)s_regs.h"

enum encore_direction {
	READ,
	WRITE,
};

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
	struct vmeio_riob riob, *riobp = &riob;
	int wordsize = reg_wnum(reg);

	riobp->mapnum = reg->block_address_space;
	riobp->offset = reg->offset;
	riobp->wsize  = reg->depth;
	riobp->buffer = buffer;
	riobp->data_width = wordsize;

	if (direction == WRITE)
		return ioctl(fd, VMEIO_RAW_WRITE, riobp);
	else if (direction == READ)
		return ioctl(fd, VMEIO_RAW_READ, riobp);
	else
		return -EINVAL;
}

static int get_set_window(int fd, 
	struct encore_reginfo *reg,
	void *buffer, int from, int to, 
	enum encore_direction direction)
{
	struct vmeio_riob riob, *riobp = &riob;
	int wordsize = reg_wnum(reg);
	int bytesize = wordsize/8;

	riobp->mapnum = reg->block_address_space;
	riobp->offset = reg->offset + from * bytesize;
	riobp->wsize  = to-from;
	riobp->buffer = buffer;
	riobp->data_width = wordsize;

	if (direction == WRITE)
		return ioctl(fd, VMEIO_RAW_WRITE, riobp);
	else if (direction == READ)
		return ioctl(fd, VMEIO_RAW_READ, riobp);
	else
		return -EINVAL;
}


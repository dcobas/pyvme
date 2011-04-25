struct encore_reginfo {
	char	name[17];
	char	rwmode[5];
	int	block;
	int	block_address_space;
	int	block_offset;
	int	register_offset;
	int	offset;
	char	wordsize[7];
	int	depth;
	char	description[81];
	int	data_width;
};

#include <sys/ioctl.h>
#include <errno.h>
#include "../../driver/vmeio.h"

extern struct encore_reginfo rf_vtu_registers[];

/* get/set for single r/w register */
int rf_vtu_get_Control_1(int fd, short *buf);
int rf_vtu_set_Control_1(int fd, short value);

/* get for single ro register */
int rf_vtu_get_Status(int fd, short *buf);

/* set for single wo register */
int rf_vtu_set_TDC11(int fd, short value);

/* get/set and window for array r/w register */
int rf_vtu_get_B_Offline(int fd, short buf[]);
int rf_vtu_set_B_Offline(int fd, short buf[]);
int rf_vtu_get_B_Offline_window(int fd, short buf[], int from, int to);
int rf_vtu_set_B_Offline_window(int fd, short buf[], int from, int to);

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

static int get_set_register_window(int fd, 
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

int rf_vtu_get_Control_1(int fd, short *buf)
{
	struct encore_reginfo *reg = &rf_vtu_registers[0];
	return get_set_register(fd, reg, buf, READ);
}

int rf_vtu_set_Control_1(int fd, short buf)
{
	struct encore_reginfo *reg = &rf_vtu_registers[0];
	return get_set_register(fd, reg, &buf, WRITE);
}

int rf_vtu_get_B_Offline(int fd, short buf[])
{
	struct encore_reginfo *reg = &rf_vtu_registers[58];
	return get_set_register(fd, reg, buf, READ);
}

int rf_vtu_get_B_Offline_window(int fd, short buf[], int from, int to)
{
	struct encore_reginfo *reg = &rf_vtu_registers[58];
	return module_get_set_window(fd, reg, buf, from, to, READ);
}

int rf_vtu_set_B_Offline_window(int fd, short buf[], int from, int to)
{
	struct encore_reginfo *reg = &rf_vtu_registers[58];
	return module_get_set_window(fd, reg, buf, from, to, WRITE);
}

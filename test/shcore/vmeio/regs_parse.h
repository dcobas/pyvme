#ifndef VMEIO_REGS_PARSE_H
#define VMEIO_REGS_PARSE_H

#define READ 	1 << 0
#define WRITE	1 << 1

struct vmeio_regs {
	char name[128];
	int flags;
	unsigned int offset;
	int size;
	int window;
	int depth;
	struct vmeio_regs *next;
};

extern struct vmeio_regs *regs;

int parse_regs(char *drv_name);

#endif

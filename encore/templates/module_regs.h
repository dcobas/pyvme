#ifndef _%(device_name)s_REGS_H
#define _%(device_name)s_REGS_H

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

extern struct encore_reginfo %(driver_name)s_registers[];
extern int %(driver_name)s_nregs;

enum %(driver_name)s_register_id {
%(register_names)s
};

#endif /* _%(device_name)s_REGS_H */

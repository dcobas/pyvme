#ifndef _%(device_name)s_REGS_H
#define _%(device_name)s_REGS_H

/* struct encore_reginfo definition needed */

extern struct encore_reginfo %(driver_name)s_registers[];
extern int %(driver_name)s_nregs;

enum %(driver_name)s_register_id {
%(register_names)s
};

#endif /* _%(device_name)s_REGS_H */

/* lib_%(driver_name)s API */
#ifndef _LIB_${module_name}_H
#define _LIB_${module_name}_H

#ifdef __cplusplus
extern "C" {
#endif

/* ${driver_name} lib declarations */

int ${driver_name}_open(int lun);
int ${driver_name}_close(int fd);
int ${driver_name}_raw(int fd, int mapping,
	unsigned offset, unsigned items, unsigned data_width,
	void *buffer, int write);

/* accessor prototypes */
${get_set_prototypes}

#ifdef __cplusplus
}
#endif
#endif /* _%(device_name)s_REGS_H */

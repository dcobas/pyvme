#ifndef _LIBENCORE_H
#define _LIBENCORE_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct encore_handle {
	int			fd;
	int			nregs;
	struct encore_reginfo	*reginfo;
} *encore_handle;

encore_handle encore_open(char *devname, int lun);
int encore_close(encore_handle h);
int encore_set_timeout(encore_handle h, int timeout);
int encore_get_timeout(encore_handle h, int *timeout);
int encore_wait(encore_handle h);

int encore_reg_id(encore_handle h, char *regname);
int encore_read(encore_handle h, int reg_id, unsigned int *value);
int encore_write(encore_handle h, int reg_id, unsigned int value);
int encore_read_window(encore_handle h, int reg_id, int from, int to, 
					void *buffer);
int encore_write_window(encore_handle h, int reg_id, int from, int to, 
					void *buffer);
int encore_raw_read(encore_handle h, int map, 
	unsigned offset, int size, int data_width, void *dst);
int encore_raw_write(encore_handle h, int map, 
	unsigned offset, int size, int data_width, void *src);
int encore_dma_read(encore_handle h, unsigned long address,
	unsigned am, unsigned data_width, unsigned long size,
	void *dst);
int encore_dma_write(encore_handle h, unsigned long address,
	unsigned am, unsigned data_width, unsigned long size,
	void *src);

#ifdef __cplusplus
}
#endif
#endif /* _LIBENCORE_H */

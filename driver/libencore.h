#ifndef _LIBENCORE_H
#define	_LIBENCORE_H	1

#ifdef __cplusplus
extern "C" {
#endif 

#define MAX_FILENAME	256

struct encore_interrupt {
	int	lun;
	int	interrupt_count;
	int	interrupt_mask;
};

int encore_open(const char *device_name, int lun);
int encore_close(int fd);
int encore_raw_read(int fd, int map, unsigned offset, unsigned bytes, void *buffer);
int encore_raw_write(int fd, int map, unsigned offset, unsigned bytes, void *buffer);
int encore_wait(int fd, unsigned int ms_timeout, struct encore_interrupt *);
int encore_dma_read(int fd, int map, unsigned offset, unsigned bytes, void *buffer);
int encore_dma_write(int fd, int map, unsigned offset, unsigned bytes, void *buffer);

#ifdef __cplusplus
}
#endif 

#endif /* _LIBENCORE_H */

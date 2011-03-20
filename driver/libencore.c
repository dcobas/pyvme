#include <sys/ioctl.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

#include "vmeio.h"
#include "libencore.h"

int encore_open(const char *device_name, int lun)
{
	char filename[MAX_FILENAME];
	int cnt;

	cnt = snprintf(filename, MAX_FILENAME, "%s.%d", device_name, lun);
	if  (cnt >= MAX_FILENAME) {
		printf("encore_open: bad parameters %s.%d\n", device_name, lun);
		return -ENODEV;
	}
	return open(filename, O_RDWR);
}

int encore_close(int fd)
{
	return close(fd);
}

static int encore_raw(int fd, int map, unsigned offset, unsigned bytes, void *buffer, int write)
{
	struct vmeio_riob_s cb;

	cb.mapnum = map;
	cb.offset = offset;
	cb.bsize = bytes;
	cb.buffer = buffer;

	if (write)
		return ioctl(fd, VMEIO_RAW_WRITE, &cb);
	else
		return ioctl(fd, VMEIO_RAW_WRITE, &cb);
}

int encore_raw_write(int fd, int map, unsigned offset, unsigned bytes, void *buffer)
{
	return encore_raw(fd, map, offset, bytes, buffer, 1);
}

int encore_raw_read(int fd, int map, unsigned offset, unsigned bytes, void *buffer)
{
	return encore_raw(fd, map, offset, bytes, buffer, 0);
}

int encore_wait(int fd, unsigned int ms_timeout, 
		struct encore_interrupt *intdata)
{
	int cc;
	struct vmeio_read_buf_s event;

	cc = ioctl(fd, VMEIO_SET_TIMEOUT, &ms_timeout);
	if (cc < 0)
		return cc;

	cc = read(fd, &event, sizeof(event));
	if (cc < 0)
		return cc;

	intdata->interrupt_mask = event.interrupt_mask;
	intdata->interrupt_count = event.interrupt_count;
	
	return 0;
}

int encore_dma_read(int fd, int map, unsigned offset, unsigned bytes, void *buffer)
{
	return -ENOSYS;
}

int encore_dma_write(int fd, int map, unsigned offset, unsigned bytes, void *buffer)
{
	return -ENOSYS;
}

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <vmeio_support.h>

#define VMEIO_MAX_DEVICES 32

char *driver_name = NULL; /* name of driver for all devices */

struct vmeio_device {
	int lun; /* logical unit number of device */
	int dma; /* whether dma is on or not */
	int dmaswap; /* not sure... */
	int window; /* window number to use */
	int dwidth; /* data width (IN BYTES!) */
	int debug; /* debug level */
	int timeout; /* timeout in msec */
	struct __vsl_device *device; /* support library structure */
};

struct vmeio_device *devices[VMEIO_MAX_DEVICES] = { NULL };

int cmd_vmeio_open(int argc, char *argv[])
{
	int lun;
	struct vmeio_device *dev;
	if (driver_name == NULL) {
		printf("driver not set (use 'driver' command)\n");
		return 0;
	}
	if (argc < 2) {
		printf("using default lun: 0\n");
		lun = 0;
	} else {
		lun = atoi(argv[1]);
	}
	if (lun < 0 || lun >= VMEIO_MAX_DEVICES - 1) {
		printf("invalid lun: %d\n", lun);
		return 0;
	}
	if (devices[lun] != NULL) {
		printf("lun %d already open\n", lun);
		return 0;
	}
	dev = malloc(sizeof(struct vmeio_device));
	if (!dev) {
		printf("unable to allocate\n"); 
		return 0;
	}
	dev->device = __vsl_open_name(lun, driver_name);
	if (!dev->device) {
		printf("failed to open /dev/%s.%d\n", driver_name, lun);
		free(dev);
		return 0;
	}
	dev->lun = lun;
	dev->dma = dev->device->dmaflag;
	dev->dmaswap = dev->device->dmaswap;
	dev->window = dev->device->mapnum;
	dev->dwidth = 4;
	dev->debug = 0;
	dev->timeout = 1000;
	devices[lun] = dev;
	printf("opened lun: %d\n", lun);
	return 0;
}

int cmd_vmeio_driver(int argc, char *argv[])
{
	int i;
	if (argc < 2) {
		if (!driver_name)
			printf("driver name not set\n");
		else
			printf("driver name: %s\n", driver_name);
		return 0;
	}
	if (driver_name) {
		for (i = 0; i < VMEIO_MAX_DEVICES; i++) {
			if (devices[i]) {
				__vsl_close(devices[i]->device);
				free(devices[i]);
				devices[i] = NULL;
			}
		}
	}
	driver_name = malloc(strlen(argv[1]) + 1);
	if (!driver_name) {
		printf("failed to allocate driver name\n");
		return 0;
	}
	strcpy(driver_name, argv[1]);
	printf("driver name set: %s\n", driver_name);
	return 0;
}


int set_dma(int argc, char *argv[])
{
	int lun;
	int dma, dmaswap;
	if (argc != 5) {
		printf("usage: %s LUN dma DMA_VALUE DMASWAP_VALUE\n", argv[0]);
		printf("*_VALUES: 0 is false, 1 is true\n");
		return 0;
	}
	lun = atoi(argv[1]);
	if (lun < 0 || lun >= VMEIO_MAX_DEVICES) {
		printf("invalid lun %d\n", lun);
		return 0;
	}
	if (!devices[lun]) {
		printf("device with LUN %d not opened\n", lun);
		return 0;
	}
	dma = atoi(argv[3]) ? 1 : 0;
	dmaswap = atoi(argv[3]) ? 1 : 0;
	if (!__vsl_set_params(devices[lun]->device, devices[lun]->window, dma, dmaswap)) {
		printf("error: failed to set dma flags\n");
		return 0;
	}
	devices[lun]->dma = dma;
	devices[lun]->dmaswap = dmaswap;
	return 0;
}

int set_debug(int argc, char *argv[])
{
	int lun;
	int debug;
	if (argc != 4) {
		printf("usage: %s LUN debug VALUE\n", argv[0]);
		printf("VALUE: debug level\n");
		return 0;
	}
	lun = atoi(argv[1]);
	if (lun < 0 || lun >= VMEIO_MAX_DEVICES) {
		printf("invalid lun %d\n", lun);
		return 0;
	}
	if (!devices[lun]) {
		printf("device with LUN %d not opened\n", lun);
		return 0;
	}
	debug = atoi(argv[3]);
	debug = (debug < 0) ? 0 : debug;
	if (!__vsl_set_debug(devices[lun]->device, &debug)) {
		printf("failed to set debug level\n");
		return 0;
	}
	devices[lun]->debug = debug;
	printf("debug for device %d set to %d\n", lun, debug);
	return 0;
}

int set_timeout(int argc, char *argv[])
{
	int lun;
	int timeout;
	if (argc != 4) {
		printf("usage: %s LUN timeout VALUE\n", argv[0]);
		printf("VALUE: timeout in msec\n");
		return 0;
	}
	lun = atoi(argv[1]);
	if (lun < 0 || lun >= VMEIO_MAX_DEVICES) {
		printf("invalid lun %d\n", lun);
		return 0;
	}
	if (!devices[lun]) {
		printf("device with LUN %d not opened\n", lun);
		return 0;
	}
	timeout = atoi(argv[3]);
	timeout = (timeout < 0) ? 0 : timeout;
	if (!__vsl_set_timeout(devices[lun]->device, &timeout)) {
		printf("failed to set timeout\n");
		return 0;
	}
	devices[lun]->timeout = timeout;
	printf("timeout for device %d set to %d\n", lun, timeout);
	return 0;
}

int set_window(int argc, char *argv[])
{
	int lun;
	int window;
	if (argc != 4) {
		printf("usage: %s LUN window VALUE\n", argv[0]);
		printf("VALUE: window number (1, 2)\n");
		return 0;
	}
	lun = atoi(argv[1]);
	if (lun < 0 || lun >= VMEIO_MAX_DEVICES) {
		printf("invalid lun %d\n", lun);
		return 0;
	}
	if (!devices[lun]) {
		printf("device with LUN %d not opened\n", lun);
		return 0;
	}
	window = atoi(argv[3]);
	if (window != 1 && window != 2) {
		printf("invalid window number. possible values are 1 and 2\n");
		return 0;
	}
	devices[lun]->window = window;
	printf("window for LUN %d set to %d\n", lun, window);
	return 0;
}

int cmd_vmeio_set(int argc, char *argv[])
{
	if (argc < 3) {
		printf("%s: usage: %s LUN [dma|debug|timeout|window] ARGS\n", argv[0], argv[0]);
		return 0;
	}
	if (strcmp(argv[2], "dma") == 0)
		return set_dma(argc, argv);
	else if (strcmp(argv[2], "debug") == 0)
		return set_debug(argc, argv);
	else if (strcmp(argv[2], "timeout") == 0)
		return set_timeout(argc, argv);
	else if (strcmp(argv[2], "window") == 0)
		return set_window(argc, argv);
	else {
		printf("invalid parameter: %s\n", argv[2]);
		return 0;
	}
}

int get_wparm(int argc, char *argv[])
{
	int lun;
	struct vmeio_get_mapping_s wp;

	printf("NOT IMPLEMENTED\n");
	return 0;

#if 0
	/* Reference */
	struct vmeio_get_mapping_s {
		int lun;     /* Logical unit number */
		int level;     /* Interrupt level */
		int vector;     /* Interrupt vector */
		int base_address1;    /* First VME base address */
		int base_address2;    /* Second VME base address or zero */
	
		int am1;     /* First address modifier */
		int am2;    /* Second address modifier or zero */
		int data_width1;    /* First data width */
		int data_width2;    /* Second data width or zero */
		int size1;   /* First map size */
		int size2;   /* Second map size or zero */
		int isrc;    /* Offset of isrc in vme1 to be read in the isr */
	};
#endif

	if (argc != 3) {
		printf("usage: %s LUN wparm\n", argv[0]);
		return 0;
	}
	lun = atoi(argv[1]);
	if (lun < 0 || lun >= VMEIO_MAX_DEVICES) {
		printf("invalid lun %d\n", lun);
		return 0;
	}
	if (!devices[lun]) {
		printf("device with LUN %d not opened\n", lun);
		return 0;
	}
	if (!__vsl_get_mapping(devices[lun]->device, &wp)) {
		printf("failed to get window parameters for LUN %d\n", lun);
		return 0;
	}
	printf("window parameters for LUN %d:\n", lun);
	if (devices[lun]->window == 2) {
		printf("interrupt level: %d\n", wp.level);
		printf("interrupt vector: %d\n", wp.vector);
		printf("base addr: %08x\n", wp.base_address2);
		printf("address mod: %02x\n", wp.am2);
		printf("data width: %d\n", wp.data_width2);
		printf("mapping size: %d\n", wp.size2);
		devices[lun]->dwidth = wp.data_width2;
	} else {
		printf("interrupt level: %d\n", wp.level);
		printf("interrupt vector: %d\n", wp.vector);
		printf("base addr: %08x\n", wp.base_address1);
		printf("address mod: %02x\n", wp.am1);
		printf("data width: %d\n", wp.data_width1);
		printf("mapping size: %d\n", wp.size1);
		devices[lun]->dwidth = wp.data_width1;
	}
	return 0;
}

int get_debug(int argc, char *argv[])
{
	int lun;
	int debug;
	if (argc != 3) {
		printf("usage: %s LUN debug\n", argv[0]);
		return 0;
	}
	lun = atoi(argv[1]);
	if (lun < 0 || lun >= VMEIO_MAX_DEVICES) {
		printf("invalid lun %d\n", lun);
		return 0;
	}
	if (!devices[lun]) {
		printf("device with LUN %d not opened\n", lun);
		return 0;
	}
	if (!__vsl_get_debug(devices[lun]->device, &debug)) {
		printf("failed to get debug level for LUN %d\n", lun);
		return 0;
	}
	printf("debug level for LUN %d: %d\n", lun, debug);
	devices[lun]->debug = debug;
	return 0;
}

int get_timeout(int argc, char *argv[])
{
	int lun;
	int timeout;
	if (argc != 3) {
		printf("usage: %s LUN timeout\n", argv[0]);
		return 0;
	}
	lun = atoi(argv[1]);
	if (lun < 0 || lun >= VMEIO_MAX_DEVICES) {
		printf("invalid lun %d\n", lun);
		return 0;
	}
	if (!devices[lun]) {
		printf("device with LUN %d not opened\n", lun);
		return 0;
	}
	if (!__vsl_get_timeout(devices[lun]->device, &timeout)) {
		printf("failed to get timeout for LUN %d\n", lun);
		return 0;
	}
	printf("timeout for LUN %d: %d msec\n", lun, timeout);
	devices[lun]->timeout = timeout;
	return 0;
}

int get_window(int argc, char *argv[])
{
	int lun;
	if (argc != 3) {
		printf("usage: %s LUN window\n", argv[0]);
		return 0;
	}
	lun = atoi(argv[1]);
	if (lun < 0 || lun >= VMEIO_MAX_DEVICES) {
		printf("invalid lun %d\n", lun);
		return 0;
	}
	if (!devices[lun]) {
		printf("device with LUN %d not opened\n", lun);
		return 0;
	}
	printf("window number for LUN %d: %d\n", lun, devices[lun]->window);
	return 0;
}


int get_version(int argc, char *argv[])
{
	int lun;
	int ver;
	if (argc != 3) {
		printf("usage: %s LUN version\n", argv[0]);
		return 0;
	}
	lun = atoi(argv[1]);
	if (lun < 0 || lun >= VMEIO_MAX_DEVICES) {
		printf("invalid lun %d\n", lun);
		return 0;
	}
	if (!devices[lun]) {
		printf("device with LUN %d not opened\n", lun);
		return 0;
	}
	if (!__vsl_get_version(devices[lun]->device, &ver)) {
		printf("failed to get version for LUN %d\n", lun);
		return 0;
	}
	printf("driver version for LUN %d: %d\n", lun, ver);
	return 0;
}

int cmd_vmeio_get(int argc, char *argv[])
{
	if (argc < 3) {
		printf("%s: usage: %s LUN [wparm|debug|timeout|window|version]\n", argv[0], argv[0]);
		return 0;
	}
	if (strcmp(argv[2], "wparm") == 0)
		return get_wparm(argc, argv);
	else if (strcmp(argv[2], "debug") == 0)
		return get_debug(argc, argv);
	else if (strcmp(argv[2], "timeout") == 0)
		return get_timeout(argc, argv);
	else if (strcmp(argv[2], "window") == 0)
		return get_window(argc, argv);
	else if (strcmp(argv[2], "version") == 0)
		return get_version(argc, argv);
	else {
		printf("invalid parameter: %s\n", argv[2]);
		return 0;
	}
}

void print_buf(unsigned int *buf, int len)
{
	int i;
	for (i = 0; i < len; i++) {
		if (i % 4 == 0 && i != 0)
			printf("\n");
		printf("%08x ", buf[i]);
	}
	printf("\n");
}

int cmd_vmeio_read(int argc, char *argv[])
{
	int lun;
	unsigned int off;
	int size;
	int dma = 0;
	unsigned int *buf; 
	struct vmeio_riob_s iob;

	if (argc != 5) {
		printf("usage: %s LUN [dma|raw] offset size\n", argv[0]);
		printf("    offset and size are hexadecimal\n");
		printf("    the current window is used\n");
		printf("    a size of 1 is 32 bits\n");
		return 0;
	}
	lun = atoi(argv[1]);
	if (lun < 0 || lun >= VMEIO_MAX_DEVICES) {
		printf("invalid lun %d\n", lun);
		return 0;
	}
	if (!devices[lun]) {
		printf("device with LUN %d not opened\n", lun);
		return 0;
	}
	off = strtoul(argv[3], NULL, 16);
	size = strtoul(argv[4], NULL, 16);

	if (size == 0) {
		printf("size is 0\n");
		return 0;
	}

	buf = malloc(size * sizeof(unsigned int));
	if (!buf) {
		printf("failed to allocate buffer\n");
		return 0;
	}
	memset(buf, 0, size*sizeof(unsigned int));

	if (strcmp(argv[2], "dma") == 0)
		dma = 1;

	iob.mapnum  = devices[lun]->window;
	iob.offset = off;
	iob.bsize  = size * sizeof(unsigned int);
	iob.buffer = (void *)buf;

	if (dma) {
		printf("DMA read of %d bytes at offset %08x (window %d):\n", size*sizeof(unsigned int), off, devices[lun]->window);
		if (!__vsl_dma(devices[lun]->device, &iob, 0)) {
			printf("Error: DMA read failed\n");
			free(buf);
			return 0;
		}
		printf("DMA Read:\n");
		print_buf(buf, size);
	} else {
		printf("RAW read of %d bytes at offset %08x (window %d):\n", size*sizeof(unsigned int), off, devices[lun]->window);
		if (!__vsl_raw(devices[lun]->device, &iob, 0)) {
			printf("Error: RAW read failed\n");
			free(buf);
			return 0;
		}
		printf("RAW Read:\n");
		print_buf(buf, size);
	}
	free(buf);
	return 0;
}

int cmd_vmeio_write(int argc, char *argv[])
{
	int lun;
	unsigned int off;
	unsigned int value;
	int dma = 0;
	unsigned int rval; 
	struct vmeio_riob_s iob;

	if (argc != 5) {
		printf("usage: %s LUN [dma|raw] offset value\n", argv[0]);
		printf("    offset is hexadecimal\n");
		printf("    the current window is used\n");
		printf("    A 32 bit value is written\n");
		return 0;
	}
	lun = atoi(argv[1]);
	if (lun < 0 || lun >= VMEIO_MAX_DEVICES) {
		printf("invalid lun %d\n", lun);
		return 0;
	}
	if (!devices[lun]) {
		printf("device with LUN %d not opened\n", lun);
		return 0;
	}
	off = strtoul(argv[3], NULL, 16);
	value = strtoul(argv[4], NULL, 16);

	if (strcmp(argv[2], "dma") == 0)
		dma = 1;

	iob.mapnum = devices[lun]->window;
	iob.offset = off;
	iob.bsize = sizeof(value);
	iob.buffer = (void *)&value;

	if (dma) {
		printf("DMA write (%d bytes) at offset %08x (window %d):\n", sizeof(unsigned int), off, devices[lun]->window);
		if (!__vsl_dma(devices[lun]->device, &iob, 1)) {
			printf("Error: DMA write failed\n");
			return 0;
		}
		printf("DMA Write Successful\n");
	} else {
		printf("RAW write (%d bytes) at offset %08x (window %d):\n", sizeof(unsigned int), off, devices[lun]->window);
		if (!__vsl_raw(devices[lun]->device, &iob, 1)) {
			printf("Error: RAW write failed\n");
			return 0;
		}
		printf("RAW Write Successful\n");
	}
	return 0;
}

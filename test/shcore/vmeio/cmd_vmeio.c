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
	printf("opening lun: %d\n", lun);
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
	int debug, debug2;
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
	if (__vsl_set_debug(devices[lun]->device, &debug)) {
		if (__vsl_get_debug(devices[lun]->device, &debug2)) {
			if (debug == debug2) {
				devices[lun]->debug = debug;
				printf("debug for device %d set to %d\n", lun, debug);
			} else
				printf("failed to set debug level\n");
		}
	}
	return 0;
}

int set_timeout(int argc, char *argv[])
{
	int lun;
	int timeout, timeout2;
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
	if (__vsl_set_timeout(devices[lun]->device, &timeout)) {
		if (__vsl_get_timeout(devices[lun]->device, &timeout2)) {
			if (timeout == timeout2) {
				devices[lun]->timeout = timeout;
				printf("timeout for device %d set to %d\n", lun, timeout);
			} else
				printf("failed to set timeout\n");
		}
	}
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
	return 0;
}

int cmd_vmeio_set(int argc, char *argv[])
{
	if (argc < 3) {
		printf("%s: usage: %s LUN [dma|debug|timeout|window] ARGS\n", argv[0], argv[0]);
		return 0;
	}
	if (strncmp(argv[2], "dma", strlen("dma")) == 0)
		return set_dma(argc, argv);
	else if (strncmp(argv[2], "debug", strlen("debug")) == 0)
		return set_debug(argc, argv);
	else if (strncmp(argv[2], "timeout", strlen("timeout")) == 0)
		return set_timeout(argc, argv);
	else if (strncmp(argv[2], "window", strlen("window")) == 0)
		return set_window(argc, argv);
	else {
		printf("invalid parameter: %s\n", argv[2]);
		return 0;
	}
}

int cmd_vmeio_version(int argc, char *argv[])
{
	int lun;
	int ver;
	if (argc != 2) {
		printf("usage: %s LUN\n", argv[0]);
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
	if (__vsl_get_version(devices[lun]->device, &ver)) {
		printf("driver version for LUN %d: %d\n", lun, ver);
	} else {
		printf("failed to get version for LUN %d\n", lun);
	}
	return 0;
}

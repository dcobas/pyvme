#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <libencore.h>

int main(int argc, char *argv[])
{
	encore_handle h;
	char *device_name;
	int lun;
	char *register_name;
	unsigned int value, readback;
	int regid;
	int opt;

	while ((opt = getopt(argc, argv, "d:U:r:v:")) != -1) {
		switch (opt) {
		case 'd':
			device_name = optarg;
			break;
		case 'U':
			lun = strtol(optarg, NULL, 0);
			break;
		case 'r':
			register_name = optarg;
			break;
		case 'v':
			value = strtol(optarg, NULL, 0);
			break;
		default: /* '?' */
			fprintf(stderr, "Usage: %s [-d device_name] [-U lun] [-r register_name] [-v value]\n", argv[0]);
			exit(EXIT_FAILURE);
		}
	}

	if ((h = encore_open(device_name, lun)) == NULL) {
		fprintf(stderr, "%s:%s: encore_open could not open %s\n",
			device_name, argv[0], device_name);
		exit(1);
	}
	regid = encore_reg_id(h, register_name);
	encore_set_register(h, regid, value);
	readback = 0;
	encore_get_register(h, regid, &readback);
	if (value != readback) {
		fprintf(stderr,
			"%s:%s: register %s: written 0x%08x, read back 0x%08x\n",
			device_name, argv[0], register_name, value, readback);
	}
	encore_close(h);
	return 0;
}

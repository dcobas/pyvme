#include <sys/ioctl.h>
#include "stdio.h"
#include "vmeio.h"

int main(int argc, char *argv[])
{
	printf("%s:\t0x%08x\n", "VMEIO_GET_DEBUG", VMEIO_GET_DEBUG);
	printf("%s:\t0x%08x\n", "VMEIO_SET_DEBUG", VMEIO_SET_DEBUG);
	printf("%s:\t0x%08x\n", "VMEIO_GET_VERSION", VMEIO_GET_VERSION);
	printf("%s:\t0x%08x\n", "VMEIO_GET_TIMEOUT", VMEIO_GET_TIMEOUT);
	printf("%s:\t0x%08x\n", "VMEIO_SET_TIMEOUT", VMEIO_SET_TIMEOUT);
	printf("%s:\t0x%08x\n", "VMEIO_RAW_READ", VMEIO_RAW_READ);
	printf("%s:\t0x%08x\n", "VMEIO_RAW_WRITE", VMEIO_RAW_WRITE);
	printf("%s:\t0x%08x\n", "VMEIO_RAW_READ_DMA", VMEIO_RAW_READ_DMA);
	printf("%s:\t0x%08x\n", "VMEIO_RAW_WRITE_DMA", VMEIO_RAW_WRITE_DMA);
	printf("%s:\t0x%08x\n", "VMEIO_READ_DMA", VMEIO_READ_DMA);
	printf("%s:\t0x%08x\n", "VMEIO_WRITE_DMA", VMEIO_WRITE_DMA);
	printf("%s:\t0x%08x\n", "VMEIO_GET_MAPPING", VMEIO_GET_MAPPING);
	printf("%s:\t0x%08x\n", "VMEIO_GET_NREGS", VMEIO_GET_NREGS);
	printf("%s:\t0x%08x\n", "VMEIO_GET_REGINFO", VMEIO_GET_REGINFO);

	return 0;
}

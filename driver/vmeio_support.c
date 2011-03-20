/**
 * =============================================================================
 * Implement library for driver to do basic VMEIO
 * This is the support layer on which hardware specific routines are implemented
 * Julian Lewis BE/CO/HT Tue 19th October 2010
 *
 */

#include <sys/ioctl.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "vmeio_support.h"

/*
 * ============================================
 * Basic routines calling driver
 */

#ifndef COMPILE_TIME
#define COMPILE_TIME 0
#endif

/**
 * ============================================
 * @brief open a handle for a given lun
 * @param lun logical unit number
 * @param name of node in /dev
 * @return handle pointer or null if error
 */

struct __vsl_device *__vsl_open_name(int lun, char *name)
{
	char fname[32];
	int fnum;
	struct __vsl_device *h;

	sprintf(fname, "/dev/%s.%1d", name, lun);
	if ((fnum = open(fname, O_RDWR, 0)) <= 0) {
		fprintf(stderr,
			"Error:%s_open Can't open:%s for read/write\n",
			DRV_NAME, fname);
		return NULL;
	}

	h = malloc(sizeof(*h));
	if (h == NULL) {
		fprintf(stderr, "Error:%s_open Can't allocate memory\n",
			DRV_NAME);
		close(fnum);
		return NULL;
	}
	memset(h, 0, sizeof(*h));

	h->file = fnum;
	h->mapnum = 1;
	h->dmaflag = 0;

	__vsl_get_mapping(h, &h->mapping);

	return h;
}

/**
 * ============================================
 * @brief close a handle
 * @param handle returned from open
 */

void __vsl_close(struct __vsl_device *h)
{
	close(h->file);
	free(h);
}

/**
 * ============================================
 * @brief Get driver and library UTC compilation dates
 * @param handle returned from open
 * @param versions of library and driver
 * @return 1 = OK 0 = FAIL
 */

int __vsl_get_version(struct __vsl_device *h, int *ver)
{
	if (ioctl(h->file, VMEIO_GET_VERSION, ver) < 0)
		return 0;
	return 1;
}

/**
 * ============================================
 * @brief Set driver timeout in milliseconds
 * @param handle returned from open
 * @param timeout in milliseconds
 * @return 1 = OK 0 = FAIL
 */

int __vsl_set_timeout(struct __vsl_device *h, int *timeout)
{
	long tmo;

	tmo = *timeout;
	if (ioctl(h->file, VMEIO_SET_TIMEOUT, &tmo) < 0)
		return 0;
	return 1;
}

/**
 * ============================================
 * @brief Set driver debug level
 * @param handle returned from open
 * @param debug level
 * @return 1 = OK 0 = FAIL
 */

int __vsl_set_debug(struct __vsl_device *h, int *level)
{
	long lvl;

	lvl = *level;
	if (ioctl(h->file, VMEIO_SET_DEBUG, &lvl) < 0)
		return 0;
	return 1;
}

/**
 * ============================================
 * @brief Get driver timeout in milliseconds
 * @param handle returned from open
 * @param timeout in milliseconds
 * @return 1 = OK 0 = FAIL
 */

int __vsl_get_timeout(struct __vsl_device *h, int *timeout)
{
	long tmo;

	if (ioctl(h->file, VMEIO_GET_TIMEOUT, &tmo) < 0)
		return 0;
	*timeout = (int) tmo;
	return 1;
}

/**
 * ============================================
 * @brief make an interrupt now
 * @param handle returned from open
 * @param mask of interrupt to make
 * @return 1 = OK 0 = FAIL
 */

int __vsl_do_interrupt(struct __vsl_device *h, int *mask)
{
	int cc;

	cc = write(h->file, mask, sizeof(int));
	if (cc < sizeof(int))
		return 0;
	return 1;
}

/**
 * ============================================
 * @brief Get driver debug level
 * @param handle returned from open
 * @param debug level
 * @return 1 = OK 0 = FAIL
 */

int __vsl_get_debug(struct __vsl_device *h, int *level)
{
	long lvl;

	if (ioctl(h->file, VMEIO_GET_DEBUG, &lvl) < 0)
		return 0;
	*level = (int) lvl;
	return 1;
}

/**
 * ============================================
 * @brief Get a window descriptor for a given lun
 * @param handle returned from open
 * @param win pointer to your descriptor
 * @return 1 = OK 0 = FAIL
 */

int __vsl_get_mapping(struct __vsl_device *h, struct vmeio_get_mapping_s *map)
{
	if (ioctl(h->file, VMEIO_GET_DEVICE, map) < 0)
		return 0;
	return 1;
}

/**
 * ============================================
 * @brief do raw IO to a memory mapped device
 * @param handle returned from open
 * @param buf is a structure containing a pointer to your data area
 * @param flag 0=read 1=write
 * @return 1 = OK 0 = FAIL
 */

int __vsl_raw(struct __vsl_device *h, struct vmeio_riob_s *buf, int flag)
{
	struct vmeio_riob_s cb;

	cb.mapnum = buf->mapnum;
	cb.offset = buf->offset;
	cb.bsize = buf->bsize;
	cb.buffer = buf->buffer;

	if (flag) {
		if (ioctl(h->file, VMEIO_RAW_WRITE, &cb) < 0)
			return 0;
	} else {
		if (ioctl(h->file, VMEIO_RAW_READ, &cb) < 0)
			return 0;
	}
	return 1;
}

/*
 * ============================================
 */

static void __vsl_swap_buf(struct __vsl_device * h, struct vmeio_riob_s *buf)
{
	int i, dwd;
	char *cp, *bp, c;

	if (h->mapnum == 2)
		dwd = h->mapping.data_width2;
	else
		dwd = h->mapping.data_width1;

	bp = buf->buffer;

	if (dwd == 1)
		return;

	if (dwd == 2) {
		for (i = 0; i < buf->bsize; i += 2) {
			cp = &bp[i];

			c = cp[1];
			cp[1] = cp[0];
			cp[0] = c;
		}
		return;
	}

	if (dwd == 4) {
		for (i = 0; i < buf->bsize; i += 4) {
			cp = &bp[i];

			c = cp[3];
			cp[3] = cp[0];
			cp[0] = c;

			c = cp[2];
			cp[2] = cp[1];
			cp[1] = c;
		}
		return;
	}
}

/**
 * ============================================
 * @brief do DMA transfer
 * @param handle returned from open
 * @param buf is a structure containing a pointer to your data area
 * @param flag 0=read 1=write
 * @return 1 = OK 0 = FAIL
 */

int __vsl_dma(struct __vsl_device *h, struct vmeio_riob_s *buf, int flag)
{
	struct vmeio_riob_s cb;

	cb.mapnum = buf->mapnum;
	cb.offset = buf->offset;
	cb.bsize = buf->bsize;
	cb.buffer = buf->buffer;

	if (flag) {
		if (h->dmaswap)
			__vsl_swap_buf(h, buf);
		if (ioctl(h->file, VMEIO_RAW_WRITE_DMA, &cb) < 0)
			return 0;
	} else {
		if (ioctl(h->file, VMEIO_RAW_READ_DMA, &cb) < 0)
			return 0;
		if (h->dmaswap)
			__vsl_swap_buf(h, buf);
	}
	return 1;
}

/**
 * ============================================
 * @brief wait for an interrupt event
 * @param handle returned from open
 * @param event will contain mask 0 if timeout occured
 * @return 1 = OK 0 = FAIL
 */

int __vsl_wait(struct __vsl_device *h, struct vmeio_read_buf_s *event)
{
	int cc;

	cc = read(h->file, event, sizeof(struct vmeio_read_buf_s));
	if (cc == -ETIME) {
		event->logical_unit = h->mapping.lun;
		event->interrupt_mask = 0;
		return 1;
	}
	if (cc < 0)
		return 0;
	return 1;
}

/*
 * ============================================
 * Convieniance routines calling the basic ones
 */

/**
 * @brief Set default parameter for DMA/READ/WRITE REG calls
 * @param handle returned from open
 * @param winnum mapping number 1..2
 * @param dmaflag 0 use map 1 use DMA
 * @param dmaswap 0 no swap 1 swap
 * @return 1 = OK 0 = FAIL
 */

int __vsl_set_params(struct __vsl_device *h, int mapnum, int dmaflag, int dmaswap)
{
	h->mapnum = mapnum;
	h->dmaflag = dmaflag;
	h->dmaswap = dmaswap;
	return 1;
}

/**
 * ============================================
 * @brief read a register
 * @param handle returned from open
 * @param reg_num register number __vsl_(not byte offset)
 * @param reg_val value read
 * @return 1 = OK 0 = FAIL
 */

int __vsl_read_reg(struct __vsl_device *h, int reg_num, int *reg_val)
{
	struct vmeio_riob_s buf;

	int cc;
	long value = 0;
	int dwd;

	if (h->mapnum == 2)
		dwd = h->mapping.data_width2;
	else
		dwd = h->mapping.data_width1;

	buf.mapnum = h->mapnum;
	buf.offset = reg_num * dwd;
	buf.bsize = dwd;
	buf.buffer = &value;

	if (h->dmaflag)
		cc = __vsl_dma(h, &buf, 0);
	else
		cc = __vsl_raw(h, &buf, 0);

	*reg_val = value;
	return cc;
}

/**
 * ============================================
 * @brief write a register
 * @param handle returned from open
 * @param reg_num register number __vsl_(not byte offset)
 * @param reg_val value read
 * @return 1 = OK 0 = FAIL
 */

int __vsl_write_reg(struct __vsl_device *h, int reg_num, int *reg_val)
{
	struct vmeio_riob_s buf;

	int cc;
	long value = 0;
	int dwd;

	value = *reg_val;

	if (h->mapnum == 2)
		dwd = h->mapping.data_width2;
	else
		dwd = h->mapping.data_width1;

	buf.mapnum = h->mapnum;
	buf.offset = reg_num * dwd;
	buf.bsize = dwd;
	buf.buffer = &value;

	if (h->dmaflag)
		cc = __vsl_dma(h, &buf, 1);
	else
		cc = __vsl_raw(h, &buf, 1);

	return cc;
}

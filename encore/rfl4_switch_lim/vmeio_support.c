/**
 * =============================================================================
 * Implement library for driver to do basic VMEIO
 * This is the support layer on which hardware specific routines are implemented
 * Julian Lewis BE/CO/HT Tue 19th October 2010
 *
 */

#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "vmeio_support.h"

/**
 * Internal handle structurs used only by the support library
 */

typedef struct {
	int file;			/** File number */
	int winum;			/** Window 1..2 */
	int dmaflag;			/** Use DMA flag 0..1 */
	int dmaswap;			/** Swap after DMA flag 0..1 */
	int offset;			/** Block offset added to all addresses */
	struct vmeio_get_window_s window;
} handle_t;

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

void *rfl4_switch_lim_open_name(int lun, char *name)
{
	char fname[32];
	int fnum;
	handle_t *h;

	sprintf(fname, "/dev/%s.%1d", name, lun);
	if ((fnum = open(fname, O_RDWR, 0)) <= 0) {
		fprintf(stderr,
			"Error:%s_open Can't open:%s for read/write\n",
			DRV_NAME, fname);
		return NULL;
	}

	h = malloc(sizeof(handle_t));
	if (h == NULL) {
		fprintf(stderr, "Error:%s_open Can't allocate memory\n",
			DRV_NAME);
		close(fnum);
		return NULL;
	}
	memset((void *) h, 0, sizeof(handle_t));

	h->file = fnum;
	h->winum = 1;
	h->dmaflag = 0;
	h->offset = 0;

	rfl4_switch_lim_get_window((void *) h, &h->window);

	return (void *) h;
}

/**
 * ============================================
 * @brief open a handle for a given lun
 * @param lun logical unit number
 * @return handle pointer or null if error
 */

void *rfl4_switch_lim_open(int lun)
{
	return rfl4_switch_lim_open_name(lun, DRV_NAME);
}

/**
 * ============================================
 * @brief close a handle
 * @param handle returned from open
 */

void rfl4_switch_lim_close(void *handle)
{
	handle_t *h = handle;
	if (h) {
		close(h->file);
		free(handle);
	}
}

/**
 * ============================================
 * @brief Get driver and library UTC compilation dates
 * @param handle returned from open
 * @param versions of library and driver
 * @return 1 = OK 0 = FAIL
 */

int rfl4_switch_lim_get_version(void *handle, struct vmeio_version_s *ver)
{

	long vd;

	handle_t *h = handle;
	if (!h)
		return 0;
	if (ioctl(h->file, VMEIO_GET_VERSION, &vd) < 0)
		return 0;
	ver->driver = vd;
	ver->library = COMPILE_TIME;
	return 1;
}

/**
 * ============================================
 * @brief Set driver timeout in milliseconds
 * @param handle returned from open
 * @param timeout in milliseconds
 * @return 1 = OK 0 = FAIL
 */

int rfl4_switch_lim_set_timeout(void *handle, int *timeout)
{
	long tmo;
	handle_t *h = handle;

	if (!h)
		return 0;
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

int rfl4_switch_lim_set_debug(void *handle, int *level)
{
	long lvl;
	handle_t *h = handle;

	if (!h)
		return 0;
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

int rfl4_switch_lim_get_timeout(void *handle, int *timeout)
{
	long tmo;
	handle_t *h = handle;

	if (!h)
		return 0;
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

int rfl4_switch_lim_do_interrupt(void *handle, int *mask)
{
	int cc;
	handle_t *h = handle;

	if (!h)
		return 0;
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

int rfl4_switch_lim_get_debug(void *handle, int *level)
{
	long lvl;
	handle_t *h = handle;

	if (!h)
		return 0;
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

int rfl4_switch_lim_get_window(void *handle,
			       struct vmeio_get_window_s *win)
{
	handle_t *h = handle;

	if (!h)
		return 0;
	if (ioctl(h->file, VMEIO_GET_DEVICE, win) < 0)
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

int rfl4_switch_lim_raw(void *handle, struct vmeio_riob_s *buf, int flag)
{
	struct vmeio_riob_s cb;
	handle_t *h = handle;

	if (!h)
		return 0;

	cb.winum = buf->winum;
	cb.offset = buf->offset + h->offset;	/* Block offset */
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

static void swap_buf(handle_t * h, struct vmeio_riob_s *buf)
{
	int i, dwd;
	char *cp, *bp, c;

	if (!h)
		return;

	if (h->winum == 2)
		dwd = h->window.dwd2;
	else
		dwd = h->window.dwd1;

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

int rfl4_switch_lim_dma(void *handle, struct vmeio_riob_s *buf, int flag)
{
	struct vmeio_riob_s cb;
	handle_t *h = handle;

	if (!h)
		return 0;

	cb.winum = buf->winum;
	cb.offset = buf->offset + h->offset;	/* Block offset */
	cb.bsize = buf->bsize;
	cb.buffer = buf->buffer;

	if (flag) {
		if (h->dmaswap)
			swap_buf(h, buf);
		if (ioctl(h->file, VMEIO_RAW_WRITE_DMA, &cb) < 0)
			return 0;
	} else {
		if (ioctl(h->file, VMEIO_RAW_READ_DMA, &cb) < 0)
			return 0;
		if (h->dmaswap)
			swap_buf(h, buf);
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

int rfl4_switch_lim_wait(void *handle, struct vmeio_read_buf_s *event)
{
	int cc;
	handle_t *h = handle;

	if (!h)
		return 0;

	cc = read(h->file, event, sizeof(struct vmeio_read_buf_s));
	if (cc == -ETIME) {
		event->logical_unit = h->window.lun;
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
 * @param winnum window number 1..2
 * @param dmaflag 0 use map 1 use DMA
 * @param dmaswap 0 no swap 1 swap
 * @return 1 = OK 0 = FAIL
 */

int rfl4_switch_lim_set_params(void *handle, int winum, int dmaflag,
			       int dmaswap)
{
	handle_t *h = handle;

	if (!h)
		return 0;

	h->winum = winum;
	h->dmaflag = dmaflag;
	h->dmaswap = dmaswap;
	h->offset = 0;
	return 1;
}

/**
 * ============================================
 * @brief read a register
 * @param handle returned from open
 * @param reg_num register number (not byte offset)
 * @param reg_val value read
 * @return 1 = OK 0 = FAIL
 */

int rfl4_switch_lim_read_reg(void *handle, int reg_num, int *reg_val)
{
	struct vmeio_riob_s buf;
	int cc;
	long value = 0;
	int dwd;
	handle_t *h = handle;

	if (!h)
		return 0;

	if (h->winum == 2)
		dwd = h->window.dwd2;
	else
		dwd = h->window.dwd1;

	buf.winum = h->winum;
	buf.offset = reg_num * dwd;
	buf.bsize = dwd;
	buf.buffer = &value;

	if (h->dmaflag)
		cc = rfl4_switch_lim_dma(handle, &buf, 0);
	else
		cc = rfl4_switch_lim_raw(handle, &buf, 0);

	*reg_val = value;
	return cc;
}

/**
 * ============================================
 * @brief write a register
 * @param handle returned from open
 * @param reg_num register number (not byte offset)
 * @param reg_val value read
 * @return 1 = OK 0 = FAIL
 */

int rfl4_switch_lim_write_reg(void *handle, int reg_num, int *reg_val)
{
	struct vmeio_riob_s buf;
	int cc;
	long value = 0;
	int dwd;
	handle_t *h = handle;

	if (!h)
		return 0;

	value = *reg_val;

	if (h->winum == 2)
		dwd = h->window.dwd2;
	else
		dwd = h->window.dwd1;

	buf.winum = h->winum;
	buf.offset = reg_num * dwd;
	buf.bsize = dwd;
	buf.buffer = &value;

	if (h->dmaflag)
		cc = rfl4_switch_lim_dma(handle, &buf, 1);
	else
		cc = rfl4_switch_lim_raw(handle, &buf, 1);

	return cc;
}

/**
 * ============================================
 * Basic support for BLOCKs needed in some modules
 * The offset is systematically added to the hardware address
 * so as to map the same set of registers onto different
 * addresses within the same logical unit.
 */

/**
 * @brief Set global block offset
 * @param handle returned from open
 * @param offset to be applied
 * @return 1 = OK 0 = FAIL
 */

int rfl4_switch_lim_set_offset(void *handle, int *offset)
{
	handle_t *h = handle;

	if (!h)
		return 0;

	h->offset = *offset;
	return 1;
}

/**
 * @brief Get global block offset
 * @param handle returned from open
 * @param will contain current offset
 * @return 1 = OK 0 = FAIL
 */

int rfl4_switch_lim_get_offset(void *handle, int *offset)
{
	handle_t *h = handle;

	if (!h)
		return 0;
	*offset = h->offset;
	return 1;
}

/**
 * =============================================================================
 * Implement library for driver to do basic VMEIO
 * This is the support layer on which hardware specific routines are implemented
 * Definitions file
 * Julian Lewis BE/CO/HT Tue 19th October 2010
 *
 */

#ifndef VMEIOSUPPORT
#define VMEIOSUPPORT

#include "vmeio.h"

/**
 * Internal open file structure used only by the support library
 * and the module library
 */

struct __vsl_device {
	int file;			/** File number */
	int mapnum;			/** Window 1..2 */
	int dmaflag;			/** Use DMA flag 0..1 */
	int dmaswap;			/** Swap after DMA flag 0..1 */
	struct vmeio_get_mapping_s mapping;
};

/*
 * ============================================
 * Basic routines calling driver
 */

/**
 * @brief open a handle for a given lun
 * @param lun logical unit number
 * @param name of the node in /dev __vsl_(used in test programs)
 * @return handle pointer or null if error
 */

struct __vsl_device *__vsl_open_name(int lun, char *name);

/**
 * @brief close a handle
 * @param handle returned from open
 */

void __vsl_close(struct __vsl_device *h);

/**
 * ============================================
 * @brief Get driver and library UTC compilation dates
 * @param handle returned from open
 * @param ver versions of library and driver
 * @return 1 = OK 0 = FAIL
 */

int __vsl_get_version(struct __vsl_device *h, int *ver);

/**
 * ============================================
 * @brief Set driver timeout in milliseconds
 * @param handle returned from open
 * @param timeout in milliseconds
 * @return 1 = OK 0 = FAIL
 */

int __vsl_set_timeout(struct __vsl_device *h, int *timeout);

/**
 * @brief Set driver debug level
 * @param handle returned from open
 * @param level of debug
 * @return 1 = OK 0 = FAIL
 */

int __vsl_set_debug(struct __vsl_device *h, int *level);

/**
 * @brief Get driver timeout in milliseconds
 * @param handle returned from open
 * @param timeout in milliseconds
 * @return 1 = OK 0 = FAIL
 */

int __vsl_get_timeout(struct __vsl_device *h, int *timeout);

/**
 * @brief Get driver debug level
 * @param handle returned from open
 * @param level of debug
 * @return 1 = OK 0 = FAIL
 */

int __vsl_get_debug(struct __vsl_device *h, int *level);

/**
 * @brief make an interrupt now
 * @param handle returned from open
 * @param mask of interrupt to make
 * @return 1 = OK 0 = FAIL
 */

int __vsl_do_interrupt(struct __vsl_device *h, int *mask);

/**
 * ============================================
 * Get a complete descriptor for a given lun
 * @param handle returned from open
 * @param win pointer to your descriptor
 * @return 1 = OK 0 = FAIL
 */

int __vsl_get_mapping(struct __vsl_device *h, struct vmeio_get_mapping_s *win);

/**
 * ============================================
 * @brief do raw IO to a memory mapped device
 * @param handle returned from open
 * @param buf is a structure containing a pointer to your data area
 * @param flag 0=read 1=write
 * @return 1 = OK 0 = FAIL
 */

int __vsl_raw(struct __vsl_device *h, struct vmeio_riob_s *buf, int flag);

/**
 * @brief Transfer data via DMA, WARNING byte swapping is your problem
 * @param handle returned from open
 * @param buf is a structure containing a pointer to your data area
 * @param flag 0=read 1=write
 * @return 1 = OK 0 = FAIL
 */

int __vsl_dma(struct __vsl_device *h, struct vmeio_riob_s *buf, int flag);

/**
 * ============================================
 * @brief wait for an interrupt event
 * @param handle returned from open
 * @param event will contain mask 0 if timeout occured
 * @return 1 = OK 0 = FAIL
 */

int __vsl_wait(struct __vsl_device *h, struct vmeio_read_buf_s *event);

/*
 * ============================================
 * Convieniance routines calling the basic ones
 */

/**
 * @brief Set default parameter for READ/WRITE REG calls
 * @param handle returned from open
 * @param mapnum mapping number 1..2
 * @param dmaflag 0 use map 1 use DMA
 * @param dmaswap 0 not swap 1 swap
 * @return 1 = OK 0 = FAIL
 */

int __vsl_set_params(struct __vsl_device *h, int mapnum, int dmaflag, int dmaswap);

/**
 * @brief read a register
 * @param handle returned from open
 * @param reg_num register number __vsl_(not byte offset)
 * @param reg_val value read
 * @return 1 = OK 0 = FAIL
 */

int __vsl_read_reg(struct __vsl_device *h, int reg_num, int *reg_val);

/**
 * @brief write a register
 * @param handle returned from open
 * @param reg_num register number __vsl_(not byte offset)
 * @param reg_val value read
 * @return 1 = OK 0 = FAIL
 */

int __vsl_write_reg(struct __vsl_device *h, int reg_num, int *reg_val);

#endif


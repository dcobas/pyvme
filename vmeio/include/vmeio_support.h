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

/*
 * This file defines the driver instance DRV_NAME and DRV_SYMB macros
 */

#include <user_driver_parameters.h>
#include <vmeio.h>

/*
 * Define library entry points as a function of the driver name
 */

#define OPEN         DRV_SYMB(_open)
#define OPEN_NAME    DRV_SYMB(_open_name)
#define CLOSE        DRV_SYMB(_close)
#define GET_WINDOW   DRV_SYMB(_get_window)
#define RAW          DRV_SYMB(_raw)
#define DMA          DRV_SYMB(_dma)
#define WAIT         DRV_SYMB(_wait)
#define SET_PARAMS   DRV_SYMB(_set_params)
#define READ_REG     DRV_SYMB(_read_reg)
#define WRITE_REG    DRV_SYMB(_write_reg)

#define SET_DEBUG    DRV_SYMB(_set_debug)
#define GET_DEBUG    DRV_SYMB(_get_debug)

#define DO_INTERRUPT DRV_SYMB(_do_interrupt)

#define SET_TIMEOUT  DRV_SYMB(_set_timeout)
#define GET_TIMEOUT  DRV_SYMB(_get_timeout)

#define GET_VERSION  DRV_SYMB(_get_version)

#define SET_OFFSET   DRV_SYMB(_set_offset)
#define GET_OFFSET   DRV_SYMB(_get_offset)

/*
 * ============================================
 * Basic routines calling driver
 */

/**
 * @brief open a handle for a given lun
 * @param lun logical unit number
 * @return handle pointer or null if error
 */

void *OPEN(int lun);

/**
 * @brief open a handle for a given lun
 * @param lun logical unit number
 * @param name of the node in /dev (used in test programs)
 * @return handle pointer or null if error
 */

void *OPEN_NAME(int lun, char *name);

/**
 * @brief close a handle
 * @param handle returned from open
 */

void CLOSE(void *handle);

/**
 * ============================================
 * @brief Get driver and library UTC compilation dates
 * @param handle returned from open
 * @param ver versions of library and driver
 * @return 1 = OK 0 = FAIL
 */

int GET_VERSION(void *handle, struct vmeio_version_s *ver);

/**
 * ============================================
 * @brief Set driver timeout in milliseconds
 * @param handle returned from open
 * @param timeout in milliseconds
 * @return 1 = OK 0 = FAIL
 */

int SET_TIMEOUT(void *handle, int *timeout);

/**
 * @brief Set driver debug level
 * @param handle returned from open
 * @param level of debug
 * @return 1 = OK 0 = FAIL
 */

int SET_DEBUG(void *handle, int *level);

/**
 * @brief Get driver timeout in milliseconds
 * @param handle returned from open
 * @param timeout in milliseconds
 * @return 1 = OK 0 = FAIL
 */

int GET_TIMEOUT(void *handle, int *timeout);

/**
 * @brief Get driver debug level
 * @param handle returned from open
 * @param level of debug
 * @return 1 = OK 0 = FAIL
 */

int GET_DEBUG(void *handle, int *level);

/**
 * @brief make an interrupt now
 * @param handle returned from open
 * @param mask of interrupt to make
 * @return 1 = OK 0 = FAIL
 */

int DO_INTERRUPT(void *handle, int *mask);

/**
 * ============================================
 * Get a window descriptor for a given lun
 * @param handle returned from open
 * @param win pointer to your descriptor
 * @return 1 = OK 0 = FAIL
 */

int GET_WINDOW(void *handle, struct vmeio_get_window_s *win);

/**
 * ============================================
 * @brief do raw IO to a memory mapped device
 * @param handle returned from open
 * @param buf is a structure containing a pointer to your data area
 * @param flag 0=read 1=write
 * @return 1 = OK 0 = FAIL
 */

int RAW(void *handle, struct vmeio_riob_s *buf, int flag);

/**
 * @brief Transfer data via DMA, WARNING byte swapping is your problem
 * @param handle returned from open
 * @param buf is a structure containing a pointer to your data area
 * @param flag 0=read 1=write
 * @return 1 = OK 0 = FAIL
 */

int DMA(void *handle, struct vmeio_riob_s *buf, int flag);

/**
 * ============================================
 * @brief wait for an interrupt event
 * @param handle returned from open
 * @param event will contain mask 0 if timeout occured
 * @return 1 = OK 0 = FAIL
 */

int WAIT(void *handle, struct vmeio_read_buf_s *event);

/*
 * ============================================
 * Convieniance routines calling the basic ones
 */

/**
 * @brief Set default parameter for READ/WRITE REG calls
 * @param handle returned from open
 * @param winnum window number 1..2
 * @param dmaflag 0 use map 1 use DMA
 * @param dmaswap 0 not swap 1 swap
 * @return 1 = OK 0 = FAIL
 */

int SET_PARAMS(void *handle, int winnum, int dmaflag, int dmaswap);

/**
 * @brief read a register
 * @param handle returned from open
 * @param reg_num register number (not byte offset)
 * @param reg_val value read
 * @return 1 = OK 0 = FAIL
 */

int READ_REG(void *handle, int reg_num, int *reg_val);

/**
 * @brief write a register
 * @param handle returned from open
 * @param reg_num register number (not byte offset)
 * @param reg_val value read
 * @return 1 = OK 0 = FAIL
 */

int WRITE_REG(void *handle, int reg_num, int *reg_val);

/**
 * ============================================
 * @brief Set global block offset
 * @param handle returned from open
 * @param offset to be applied
 * @return 1 = OK 0 = FAIL
 */

int SET_OFFSET(void *handle, int *offset);

/**
 * @brief Get global block offset
 * @param handle returned from open
 * @param offset will contain current offset
 * @return 1 = OK 0 = FAIL
 */

int GET_OFFSET(void *handle, int *offset);

#endif

/**
 * =========================================================================================
 *
 * Implement driver to do basic VME IO
 * Definitions file
 * Julian Lewis BE/CO/HT Tue 19th October 2010
 *
 * =========================================================================================
 * Basic driver API for raw VME IO
 */

#ifndef _VMEIO_H
#define _VMEIO_H

#include <vmebus.h>

#define DEBUG 0
#define TIMEOUT 1000

/*
 * driver name, used all over driver and library (bad!)
 * the way this is set up now, DRIVER_NAME is defined at compile time
 * via a -D flag and this propagates properly through vmeio.h
 */
#ifndef	DRIVER_NAME
#define DRIVER_NAME	"vmeio"
#endif
#define DRV_NAME	DRIVER_NAME
#define DRV_MAX_DEVICES	32

/**
 * Connect and read buffer
 * The interrupt counter is the total number of interrupts
 * so far on the lun, so you can know how many you missed.
 */

struct vmeio_read_buf_s {
   int logical_unit;    /** Logical unit number for interrupt */
   int interrupt_mask;  /** Interrupt enable/source mask */
   int interrupt_count; /** Current interrupt counter value */
};

/**
 * Parameter for get_mapping
 */

struct vmeio_get_mapping {
	int			mapnum;
	struct vme_mapping	map;
};

/*
 * Parameter for raw IO
 */

#define vmeioMAX_BUF 8192

struct vmeio_riob_s {
   int mapnum;   /** Mapping number 1..2 */
   int offset;   /** Byte offset in map */
   int bsize;    /** The number of bytes to read */
   void *buffer; /** Pointer to data area */
};

struct vmeio_dma_op {
   int am;		/** address modifier, defines transfer mode */
   int data_width;	/** transfer data width */
   int address;		/** vme address of start transfer */
   int byte_length;	/** transfer length in bytes */
   void *buffer;	/** pointer to transfer to/from */
   enum vme_dma_dir direction;	/** from/to device */
};

/*
 * Enumerate IOCTL functions
 */

#define PRANDOM 0

typedef enum {

   vmeioFIRST = PRANDOM,

   vmeioSET_DEBUG,     /** Debug level 0..4 */
   vmeioGET_DEBUG,

   vmeioGET_VERSION,   /** Get driver and library dates */

   vmeioSET_TIMEOUT,   /** Timeout in milliseconds */
   vmeioGET_TIMEOUT,

   vmeioGET_DEVICE,    /** Get the mappings described in struct vmeio_get_mapping_s */
   vmeioRAW_READ,      /** Raw read VME registers */
   vmeioRAW_WRITE,     /** Raw write VME registers */
   vmeioRAW_READ_DMA,  /** Raw read VME registers */
   vmeioRAW_WRITE_DMA, /** Raw write VME registers */

   vmeioSET_DEVICE,    /** Very dangerous IOCTL, not for users */
   vmeioREAD_DMA,      /** Direct interface to DMA */
   vmeioWRITE_DMA,     /** Direct interface to DMA */
   vmeioGET_MAPPING,   /** Obtain mapping properties */

   vmeioLAST           /** For range checking (LAST - FIRST) */

} vmeio_ioctl_function_t;

#define vmeioIOCTL_FUNCTIONS (vmeioLAST - vmeioFIRST - 1)

/*
 * Set up the IOCTL numbers
 */

#define MAGIC 'V'

#define  VMEIO_GET_DEBUG      _IOR(MAGIC,   vmeioGET_DEBUG,      int)
#define  VMEIO_SET_DEBUG      _IOW(MAGIC,   vmeioSET_DEBUG,      int)
#define  VMEIO_GET_VERSION    _IOR(MAGIC,   vmeioGET_VERSION,    int)
#define  VMEIO_GET_TIMEOUT    _IOR(MAGIC,   vmeioGET_TIMEOUT,    int)
#define  VMEIO_SET_TIMEOUT    _IOW(MAGIC,   vmeioSET_TIMEOUT,    int)
#define  VMEIO_RAW_READ       _IOWR(MAGIC,  vmeioRAW_READ,       struct  vmeio_riob_s)
#define  VMEIO_RAW_WRITE      _IOWR(MAGIC,  vmeioRAW_WRITE,      struct  vmeio_riob_s)
#define  VMEIO_RAW_READ_DMA   _IOWR(MAGIC,  vmeioRAW_READ_DMA,   struct  vmeio_riob_s)
#define  VMEIO_RAW_WRITE_DMA  _IOWR(MAGIC,  vmeioRAW_WRITE_DMA,  struct  vmeio_riob_s)
#define  VMEIO_READ_DMA       _IOW(MAGIC,   vmeioREAD_DMA,       struct	 vmeio_dma_op)
#define  VMEIO_WRITE_DMA      _IOW(MAGIC,   vmeioWRITE_DMA,      struct  vmeio_dma_op)
#define  VMEIO_GET_MAPPING    _IOWR(MAGIC,  vmeioGET_MAPPING,    struct  vme_mapping)

#endif

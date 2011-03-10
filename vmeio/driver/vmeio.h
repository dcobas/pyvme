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

#ifndef VMEIO
#define VMEIO

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
 * Parameter for get window
 */

struct vmeio_get_window_s {

   int lun;     /* Logical unit number */
   int lvl;     /* Interrupt level */
   int vec;     /* Interrupt vector */
   int vme1;    /* First VME base address */
   int vme2;    /* Second VME base address or zero */

   int amd1;    /* First address modifier */
   int amd2;    /* Second address modifier or zero */
   int dwd1;    /* First data width */
   int dwd2;    /* Second data width or zero */
   int win1;    /* First window size */
   int win2;    /* Second window size or zero */
   int nmap;    /* No map window flag, 1=DMA only */
   int isrc;    /* Offset of isrc in vme1 to be read in the isr */
};

/*
 * Parameter for raw IO
 */

#define vmeioMAX_BUF 8192

struct vmeio_riob_s {
   int winum;    /** Window number 1..2 */
   int offset;   /** Byte offset in map */
   int bsize;    /** The number of bytes to read */
   void *buffer; /** Pointer to data area */
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

   vmeioGET_DEVICE,    /** Get the window described in struct vmeio_get_window_s */
   vmeioRAW_READ,      /** Raw read VME registers */
   vmeioRAW_WRITE,     /** Raw write VME registers */
   vmeioRAW_READ_DMA,  /** Raw read VME registers */
   vmeioRAW_WRITE_DMA, /** Raw write VME registers */

   vmeioSET_DEVICE,    /** Very dangerous IOCTL, not for users */

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
#define  VMEIO_GET_DEVICE     _IOR(MAGIC,   vmeioGET_DEVICE,     struct  vmeio_get_window_s)
#define  VMEIO_RAW_READ       _IOWR(MAGIC,  vmeioRAW_READ,       struct  vmeio_riob_s)
#define  VMEIO_RAW_WRITE      _IOWR(MAGIC,  vmeioRAW_WRITE,      struct  vmeio_riob_s)
#define  VMEIO_RAW_READ_DMA   _IOWR(MAGIC,  vmeioRAW_READ_DMA,   struct  vmeio_riob_s)
#define  VMEIO_RAW_WRITE_DMA  _IOWR(MAGIC,  vmeioRAW_WRITE_DMA,  struct  vmeio_riob_s)
#define  VMEIO_SET_DEVICE     _IOW(MAGIC,   vmeioGET_DEVICE,     struct  vmeio_get_window_s)

#endif

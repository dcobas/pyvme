/**
 * =============================================================================
 * Implement library for driver to do basic VMEIO
 * This is the support layer on which hardware specific routines are implemented
 * Julian Lewis BE/CO/HT Tue 19th October 2010
 *
 */

#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sched.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/file.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <dlfcn.h>
#include <unistd.h>
#include <errno.h>

#include <vmeio_support.h>

/**
 * Internal handle structurs used only by the support library
 */

typedef struct {
   int                       file;      /** File number */
   int                       winum;     /** Window 1..2 */
   int                       dmaflag;   /** Use DMA flag 0..1 */
   int                       dmaswap;   /** Swap after DMA flag 0..1 */
   int                       offset;    /** Block offset added to all addresses */
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

void *OPEN_NAME(int lun, char *name) {

char fname[32];
int fnum;
handle_t *h;

   sprintf(fname,"/dev/%s.%1d",name,lun);
   if ((fnum = open(fname,O_RDWR,0)) <= 0) {
      fprintf(stderr,"Error:%s_open Can't open:%s for read/write\n",DRV_NAME,fname);
      return NULL;
   }

   h = (handle_t *) malloc(sizeof(handle_t));
   if (h == NULL) {
      fprintf(stderr,"Error:%s_open Can't allocate memory\n",DRV_NAME);
      close(fnum);
      return NULL;
   }
   bzero((void *) h, sizeof(handle_t));

   h->file = fnum;
   h->winum = 1;
   h->dmaflag = 0;
   h->offset = 0;

   GET_WINDOW((void *) h, &h->window);

   return (void *) h;
}

/**
 * ============================================
 * @brief open a handle for a given lun
 * @param lun logical unit number
 * @return handle pointer or null if error
 */

void *OPEN(int lun) {

   return OPEN_NAME(lun,DRV_NAME);
}

/**
 * ============================================
 * @brief close a handle
 * @param handle returned from open
 */

void CLOSE(void *handle) {

handle_t *h;

   h = (handle_t *) handle;
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

int GET_VERSION(void *handle, struct vmeio_version_s *ver) {

handle_t *h;
long vd;

   h = (handle_t *) handle;
   if (!h) return 0;
   if (ioctl(h->file,VMEIO_GET_VERSION,&vd) <0) return 0;
   ver->driver  = vd;
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

int SET_TIMEOUT(void *handle, int *timeout) {

handle_t *h;
long tmo;

   h = (handle_t *) handle;
   if (!h) return 0;
   tmo = *timeout;
   if (ioctl(h->file,VMEIO_SET_TIMEOUT,&tmo) <0) return 0;
   return 1;
}

/**
 * ============================================
 * @brief Set driver debug level
 * @param handle returned from open
 * @param debug level
 * @return 1 = OK 0 = FAIL
 */

int SET_DEBUG(void *handle, int *level) {

handle_t *h;
long lvl;

   h = (handle_t *) handle;
   if (!h) return 0;
   lvl = *level;
   if (ioctl(h->file,VMEIO_SET_DEBUG,&lvl) <0) return 0;
   return 1;
}

/**
 * ============================================
 * @brief Get driver timeout in milliseconds
 * @param handle returned from open
 * @param timeout in milliseconds
 * @return 1 = OK 0 = FAIL
 */

int GET_TIMEOUT(void *handle, int *timeout) {

handle_t *h;
long tmo;

   h = (handle_t *) handle;
   if (!h) return 0;
   if (ioctl(h->file,VMEIO_GET_TIMEOUT,&tmo) <0) return 0;
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

int DO_INTERRUPT(void *handle, int *mask) {

handle_t *h;
int cc;

   h = (handle_t *) handle;
   if (!h) return 0;
   cc = write(h->file,mask,sizeof(int));
   if (cc < sizeof(int)) return 0;
   return 1;
}

/**
 * ============================================
 * @brief Get driver debug level
 * @param handle returned from open
 * @param debug level
 * @return 1 = OK 0 = FAIL
 */

int GET_DEBUG(void *handle, int *level) {

handle_t *h;
long lvl;

   h = (handle_t *) handle;
   if (!h) return 0;
   if (ioctl(h->file,VMEIO_GET_DEBUG,&lvl) <0) return 0;
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

int GET_WINDOW(void *handle, struct vmeio_get_window_s *win) {

handle_t *h;

   h = (handle_t *) handle;
   if (!h) return 0;
   if (ioctl(h->file,VMEIO_GET_DEVICE,win) <0) return 0;
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

int RAW(void *handle, struct vmeio_riob_s *buf, int flag) {

handle_t *h;
struct vmeio_riob_s cb;

   h = (handle_t *) handle;
   if (!h) return 0;

   cb.winum  = buf->winum;
   cb.offset = buf->offset + h->offset;  /* Block offset */
   cb.bsize  = buf->bsize;
   cb.buffer = buf->buffer;

   if (flag) {
      if (ioctl(h->file,VMEIO_RAW_WRITE,&cb) <0) return 0;
   } else {
      if (ioctl(h->file,VMEIO_RAW_READ,&cb) <0) return 0;
   }
   return 1;
}

/*
 * ============================================
 */

static void swap_buf(handle_t *h, struct vmeio_riob_s *buf) {

int i, dwd;
char *cp, *bp, c;

   if (!h) return;

   if (h->winum == 2) dwd = h->window.dwd2;
   else               dwd = h->window.dwd1;

   bp = buf->buffer;

   if (dwd == 1) return;

   if (dwd == 2) {
      for (i=0; i<buf->bsize; i+=2) {
	 cp = &bp[i];

	 c = cp[1];
	 cp[0] = cp[1];
	 cp[1] = c;
      }
      return;
   }

   if (dwd == 4) {
      for (i=0; i<buf->bsize; i+=4) {
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

int DMA(void *handle, struct vmeio_riob_s *buf, int flag) {

handle_t *h;
struct vmeio_riob_s cb;

   h = (handle_t *) handle;
   if (!h) return 0;

   cb.winum  = buf->winum;
   cb.offset = buf->offset + h->offset;  /* Block offset */
   cb.bsize  = buf->bsize;
   cb.buffer = buf->buffer;

   if (flag) {
      if (h->dmaswap) swap_buf(h,buf);
      if (ioctl(h->file,VMEIO_RAW_WRITE_DMA,&cb) <0) return 0;
   } else {
      if (ioctl(h->file,VMEIO_RAW_READ_DMA,&cb) <0) return 0;
      if (h->dmaswap) swap_buf(h,buf);
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

int WAIT(void *handle, struct vmeio_read_buf_s *event) {

handle_t *h;
int cc;

   h = (handle_t *) handle;
   if (!h) return 0;

   cc = read(h->file,event,sizeof(struct vmeio_read_buf_s));
   if (cc == -ETIME) {
      event->logical_unit = h->window.lun;
      event->interrupt_mask = 0;
      return 1;
   }
   if (cc < 0) return 0;
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

int SET_PARAMS(void *handle, int winum, int dmaflag, int dmaswap) {

handle_t *h;

   h = (handle_t *) handle;
   if (!h) return 0;

   h->winum   = winum;
   h->dmaflag = dmaflag;
   h->dmaswap = dmaswap;
   h->offset  = 0;
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

int READ_REG(void *handle, int reg_num, int *reg_val) {

handle_t *h;
struct vmeio_riob_s buf;

int cc;
long value = 0;
int dwd;

   h = (handle_t *) handle;
   if (!h) return 0;

   if (h->winum == 2) dwd = h->window.dwd2;
   else               dwd = h->window.dwd1;

   buf.winum  = h->winum;
   buf.offset = reg_num * dwd;
   buf.bsize  = dwd;
   buf.buffer = &value;

   if (h->dmaflag) cc = DMA(handle,&buf,0);
   else            cc = RAW(handle,&buf,0);

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

int WRITE_REG(void *handle, int reg_num, int *reg_val) {

handle_t *h;
struct vmeio_riob_s buf;

int cc;
long value = 0;
int dwd;

   h = (handle_t *) handle;
   if (!h) return 0;

   value = *reg_val;

   if (h->winum == 2) dwd = h->window.dwd2;
   else               dwd = h->window.dwd1;

   buf.winum  = h->winum;
   buf.offset = reg_num * dwd;
   buf.bsize  = dwd;
   buf.buffer = &value;

   if (h->dmaflag) cc = DMA(handle,&buf,1);
   else            cc = RAW(handle,&buf,1);

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

int SET_OFFSET(void *handle, int *offset) {

handle_t *h;

   h = (handle_t *) handle;
   if (!h) return 0;

   h->offset = *offset;
   return 1;
}

/**
 * @brief Get global block offset
 * @param handle returned from open
 * @param will contain current offset
 * @return 1 = OK 0 = FAIL
 */

int GET_OFFSET(void *handle, int *offset) {

handle_t *h;

   h = (handle_t *) handle;
   if (!h) return 0;
   *offset = h->offset;
   return 1;
}

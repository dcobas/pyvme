#include <vmeio_support.h>


/**
 * Library for CVORA module 
 * Generated automatically by genlib.c.awk at:2010 Thu November-11 11:12:47
 */

#include <libcvora.h>


int SET_CONTROL(void *handle, int *control) {
   int offset = 0;
   struct vmeio_riob_s riob;
   riob.winum  =1;
   riob.offset =0x00 + offset;
   riob.bsize  =4;
   riob.buffer =control;
   return DMA(handle,&riob,1);
}


int GET_STATUS(void *handle, int *status) {
   int offset = 0;
   struct vmeio_riob_s riob;
   riob.winum  =1;
   riob.offset =0x00 + offset;
   riob.bsize  =4;
   riob.buffer =status;
   return DMA(handle,&riob,0);
}


int SET_MEM_POINTER(void *handle, int *mem_pointer) {
   int offset = 0;
   struct vmeio_riob_s riob;
   riob.winum  =1;
   riob.offset =0x04 + offset;
   riob.bsize  =4;
   riob.buffer =mem_pointer;
   return DMA(handle,&riob,1);
}

int GET_MEM_POINTER(void *handle, int *mem_pointer) {
   int offset = 0;
   struct vmeio_riob_s riob;
   riob.winum  =1;
   riob.offset =0x04 + offset;
   riob.bsize  =4;
   riob.buffer =mem_pointer;
   return DMA(handle,&riob,0);
}


int SET_MODE(void *handle, int *mode) {
   int offset = 0;
   struct vmeio_riob_s riob;
   riob.winum  =1;
   riob.offset =0x08 + offset;
   riob.bsize  =4;
   riob.buffer =mode;
   return DMA(handle,&riob,1);
}


int GET_MODULE_ID(void *handle, int *module_id) {
   int offset = 0;
   struct vmeio_riob_s riob;
   riob.winum  =1;
   riob.offset =0x08 + offset;
   riob.bsize  =4;
   riob.buffer =module_id;
   return DMA(handle,&riob,0);
}


int GET_CHANNELS(void *handle, int *channels) {
   int offset = 0;
   struct vmeio_riob_s riob;
   riob.winum  =1;
   riob.offset =0x0C + offset;
   riob.bsize  =4;
   riob.buffer =channels;
   return DMA(handle,&riob,0);
}


int GET_FREQ(void *handle, int *freq) {
   int offset = 0;
   struct vmeio_riob_s riob;
   riob.winum  =1;
   riob.offset =0x10 + offset;
   riob.bsize  =4;
   riob.buffer =freq;
   return DMA(handle,&riob,0);
}


int SET_PLOT(void *handle, int *plot) {
   int offset = 0;
   struct vmeio_riob_s riob;
   riob.winum  =1;
   riob.offset =0x10 + offset;
   riob.bsize  =4;
   riob.buffer =plot;
   return DMA(handle,&riob,1);
}


int GET_DAC(void *handle, int *dac) {
   int offset = 0;
   struct vmeio_riob_s riob;
   riob.winum  =1;
   riob.offset =0x14 + offset;
   riob.bsize  =4;
   riob.buffer =dac;
   return DMA(handle,&riob,0);
}


int SET_RAM(void *handle, int offset, int *ram) {
   struct vmeio_riob_s riob;
   riob.winum  =1;
   riob.offset =0x20 + offset;
   riob.bsize  =4;
   riob.buffer =ram;
   return DMA(handle,&riob,1);
}

int GET_RAM(void *handle, int offset, int *ram) {
   struct vmeio_riob_s riob;
   riob.winum  =1;
   riob.offset =0x20 + offset;
   riob.bsize  =4;
   riob.buffer =ram;
   return DMA(handle,&riob,0);
}


int SET_JTAG(void *handle, short *jtag) {
   int offset = 0;
   struct vmeio_riob_s riob;
   riob.winum  =2;
   riob.offset =0x00 + offset;
   riob.bsize  =2;
   riob.buffer =jtag;
   return DMA(handle,&riob,1);
}

int GET_JTAG(void *handle, short *jtag) {
   int offset = 0;
   struct vmeio_riob_s riob;
   riob.winum  =2;
   riob.offset =0x00 + offset;
   riob.bsize  =2;
   riob.buffer =jtag;
   return DMA(handle,&riob,0);
}



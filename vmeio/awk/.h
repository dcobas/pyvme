#include <vmeio_support.h>

#ifndef GEN_LIB_API
#define GEN_LIB_API

/**
 * API for CVORA module 
 * Generated automatically by genlib.h.awk at:2010 Thu November-11 11:12:47
 */

#define SET_CONTROL DRV_SYMB(_set_control)
int SET_CONTROL(void *handle, int *control);

#define GET_STATUS DRV_SYMB(_get_status)
int GET_STATUS(void *handle, int *status);

#define SET_MEM_POINTER DRV_SYMB(_set_mem_pointer)
int SET_MEM_POINTER(void *handle, int *mem_pointer);
#define GET_MEM_POINTER DRV_SYMB(_get_mem_pointer)
int GET_MEM_POINTER(void *handle, int *mem_pointer);

#define SET_MODE DRV_SYMB(_set_mode)
int SET_MODE(void *handle, int *mode);

#define GET_MODULE_ID DRV_SYMB(_get_module_id)
int GET_MODULE_ID(void *handle, int *module_id);

#define GET_CHANNELS DRV_SYMB(_get_channels)
int GET_CHANNELS(void *handle, int *channels);

#define GET_FREQ DRV_SYMB(_get_freq)
int GET_FREQ(void *handle, int *freq);

#define SET_PLOT DRV_SYMB(_set_plot)
int SET_PLOT(void *handle, int *plot);

#define GET_DAC DRV_SYMB(_get_dac)
int GET_DAC(void *handle, int *dac);

#define SET_RAM DRV_SYMB(_set_ram)
int SET_RAM(void *handle, int offset, int *ram);
#define GET_RAM DRV_SYMB(_get_ram)
int GET_RAM(void *handle, int offset, int *ram);

#define SET_JTAG DRV_SYMB(_set_jtag)
int SET_JTAG(void *handle, short *jtag);
#define GET_JTAG DRV_SYMB(_get_jtag)
int GET_JTAG(void *handle, short *jtag);

#endif

/*
 * Definitions for the DGII CTC API
 * BE/CO/HT Thu 9th Dec 2010 Julian
 */

#ifndef CTC_USER_ACCESS
#define CTC_USER_ACCESS

#include <vmeio_support.h>

typedef void *HANDLE;

#ifndef OK
#define OK 0
#endif

typedef enum {
	CTC_CH_OUT_OF_RANGE = 100,
	CTC_VAL_OUT_OF_RANGE,
	CTC_ERR,
	CTC_DAL_ERR
} ctc_uda_err;

#define F_OUT_CH 1
#define L_OUT_CH 8

#define F_INP_CH 1
#define L_INP_CH 40

#define F_CNTR_CLK 1
#define L_CNTR_CLK 6

#define REGS_PER_CHAN 6

typedef struct _CTC_config_reg {
	int cr_ext_start;
	int cr_cntr1_clk;
	int cr_cntr2_clk;
	int cr_mode;
	int cr_direction;
} ctc_cfg_reg_t;

#define CHCFG_ESC_MASK 0x3F000000L
#define CHCFG_C1S_MASK 0x700000L
#define CHCFG_C2S_MASK 0x70000L
#define CHCFG_MOD_MASK 0x2L
#define CHCFG_DIR_MASK 0x1L

#define CHCFG_ESC_SHIFT 24
#define CHCFG_C1S_SHIFT 20
#define CHCFG_C2S_SHIFT 16
#define CHCFG_MOD_SHIFT 1
#define CHCFG_DIR_SHIFT 0

#define CHCFG_ESC_SIGBITS_MASK 0x3F
#define CHCFG_C1S_SIGBITS_MASK 0x7
#define CHCFG_C2S_SIGBITS_MASK 0x7
#define CHCFG_MOD_SIGBITS_MASK 0x2
#define CHCFG_DIR_SIGBITS_MASK 0x1

int ctc_setInputChan(HANDLE handle, int chan, unsigned long newVal);
int ctc_getClock1(HANDLE handle, int chan, unsigned long *res);
int ctc_setClock1(HANDLE handle, int chan, unsigned long newVal);
int ctc_getClock2(HANDLE handle, int chan, unsigned long *res);
int ctc_setClock2(HANDLE handle, int chan, unsigned long newVal);
int ctc_getClock1Tick(HANDLE handle, int chan, unsigned long *res);
int ctc_setClock1Tick(HANDLE handle, int chan, unsigned long newVal);
int ctc_getClock2Tick(HANDLE handle, int chan, unsigned long *res);
int ctc_setClock2Tick(HANDLE handle, int chan, unsigned long newVal);
int ctc_getOutPutCounter(HANDLE handle, int chan, unsigned long *res);
int ctc_getCntr1CurVal(HANDLE handle, int chan, unsigned long *res);
int ctc_getCntr2CurVal(HANDLE handle, int chan, unsigned long *res);
int ctc_getModuleStatus(HANDLE handle, unsigned long *res);
int ctc_resetModule(HANDLE handle);
int ctc_enableChannel(HANDLE handle, int outChan, int inpChan);
int ctc_disableChannel(HANDLE handle, int outChan);
ctc_cfg_reg_t* ctc_getChanConf(HANDLE handle, int outChan);
int ctc_dbgPrintout(int toggle);
int ctc_getChannelStatus(HANDLE handle, int outChan[8]);

#endif

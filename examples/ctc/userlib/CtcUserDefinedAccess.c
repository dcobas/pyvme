/*
 * A legacy DGII interface base to save modifying client code
 * I just do the minimun needed to get the old API working
 * Some of this looks like crap, but I strictly replicate the
 * old behaviour towards the hardware, just in case.
 *
 * Wed 8th Dec 2010 CO/HT Julian Lewis
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

/*
 * About the type HANDLE used in the old drivers genII.
 * Here it is actually a pointer to a support library
 * handle and should have been obtained from the OPEN
 * support library function. This will require a small
 * modification to the client code.
 */

#include <CtcUserDefinedAccess.h>

typedef struct {
	int confChan;
	int clock1Delay;
	int clock2Delay;
	int outputCntr;
	int cntr1CurVal;
	int cntr2CurVal;
} ctc_channel_s;

struct ctc_map_s {
	int           status;
	int           cntr_enable;
	ctc_channel_s channels[L_OUT_CH];
};

struct ctc_map_s *map = NULL; /* Just for getting addresses */

int ctc_lib_glob_dbg_flag = 0;

/**
 * =====================================================================
 * Helper routines to call RAW IO and handle errors
 */

#define DO_READ 0
#define DO_WRITE 1

static int do_io(HANDLE handle, int *offset, int *data, int flag)
{
	struct vmeio_riob_s buf;

	buf.winum  = 1;
	buf.offset = (int) offset;
	buf.bsize  = 4;
	buf.buffer = data;

	if (!RAW(handle,&buf,flag))
		return 0;
	return 1;
}

static int do_error(int err) {

	char *cp;

	if (ctc_lib_glob_dbg_flag) {
		if (err == CTC_CH_OUT_OF_RANGE)
			cp = "Channel Out of Range";
		else if (err == CTC_VAL_OUT_OF_RANGE)
			cp = "Value Out of Range";
		else if (err == CTC_ERR)
			cp = "General CTC Error";
		else if (err == CTC_DAL_ERR)
			cp = "Driver Access Library Error";
		else
			cp = "???";

		fprintf(stderr,"CtcUserDefinedAccess: Error:%d [%s]\n",err,cp);
	}
	return -err;
}

/**
 * =====================================================================
 * @brief Get input channel (i.e. external start) of output channel
 *        in question.
 *
 * @param handle - DAL handle
 * @param chan   - [1, 8] range
 * @param res    - [1, 40] external start channel
 *
 * @return OK                   - if succeed. i.e. 0
 * @return -CTC_CH_OUT_OF_RANGE - channel out of range
 * @return -CTC_DAL_ERR         - DAL return error
 */

int ctc_getInputChan(HANDLE handle, int chan, unsigned long *res)
{
	int locRes;

	if ((chan < F_OUT_CH) || (chan > L_OUT_CH))
		return do_error(CTC_CH_OUT_OF_RANGE);

	if (!do_io(handle,&(map->channels[chan - F_OUT_CH].confChan),&locRes,DO_READ))
		return do_error(CTC_DAL_ERR);

	locRes = ((locRes & CHCFG_ESC_MASK) >> CHCFG_ESC_SHIFT) + 1;
	*res = locRes;

	return OK;
}

/**
 * =====================================================================
 * @brief Input (i.e. external start) <-> output channel mapping.
 *
 * @param handle - DAL handle
 * @param chan   - output channel. [1, 8] range
 * @param newVal - input channel. [1, 40] range.
 *                 If zero - then module resetting requested.
 *                 Will be set to default external trigger (i.e. input channel)
 *                 namely the first one.
 *
 * @return OK                   - if succeed. i.e. 0
 * @return -CTC_CH_OUT_OF_RANGE - input/output chan out of range
 * @return -CTC_DAL_ERR         - DAL return error
 */

int ctc_setInputChan(HANDLE handle, int chan, unsigned long newVal)
{
	int curVal;

	if ((chan < F_OUT_CH) || (chan > L_OUT_CH))
		return do_error(CTC_CH_OUT_OF_RANGE);

	if (newVal > L_INP_CH)
		return do_error(CTC_CH_OUT_OF_RANGE);

	if (!do_io(handle,&(map->channels[chan - F_OUT_CH].confChan),&curVal,DO_READ))
		return do_error(CTC_DAL_ERR);

	curVal &= ~(CHCFG_ESC_MASK);
	if (newVal)
		curVal |= (((newVal - 1) & CHCFG_ESC_SIGBITS_MASK) << CHCFG_ESC_SHIFT);

	if (!do_io(handle,&(map->channels[chan - F_OUT_CH].confChan),&curVal,DO_WRITE))
		return do_error(CTC_DAL_ERR);

	return OK;
}

/**
 * =====================================================================
 * @brief Returns current clock setting for the first counter.
 *
 * @param handle - DAL handle
 * @param chan   - [1, 8] range
 * @param res    - reg val will be put here
 *
 * @note Real register value will be interpreted here, since the clock setting,
 *       as seen by the user is (reg_value + 1), i.e.\n
 *       0 - clock1\n
 *       1 - clock2 etc...
 *
 * @return OK                   - if succeed. i.e. 0
 * @return -CTC_CH_OUT_OF_RANGE - input/output chan out of range
 * @return -CTC_DAL_ERR         - DAL return error
 */

int ctc_getClock1(HANDLE handle, int chan, unsigned long *res)
{
	int locRes;

	if ((chan < F_OUT_CH) || (chan > L_OUT_CH))
		return do_error(CTC_CH_OUT_OF_RANGE);

	if (!do_io(handle,&(map->channels[chan - F_OUT_CH].confChan),&locRes,DO_READ))
		return do_error(CTC_DAL_ERR);

	locRes = (locRes & CHCFG_C1S_MASK) >> CHCFG_C1S_SHIFT;
	*res = locRes + 1;

	return OK;
}

/**
 * =====================================================================
 * @brief Set external clock for the first counter.
 *
 * @param handle - DAL handle
 * @param chan   - [1, 8] range
 * @param newVal - value to set [1, 6]
 *
 * @note Provided user value will be interpreted, since the the real clock
 *       setting is (user_value - 1), i.e.\n
 *       0 - clock1\n
 *       1 - clock2 etc...
 *
 * @return OK                    - if succeed. i.e. 0
 * @return -CTC_CH_OUT_OF_RANGE  - input/output chan out of range
 * @return -CTC_VAL_OUT_OF_RANGE - requested external clock is out-of-range
 * @return -CTC_DAL_ERR          - DAL return error
 */

int ctc_setClock1(HANDLE handle, int chan, unsigned long newVal)
{
	int curVal;

	if ((chan < F_OUT_CH) || (chan > L_OUT_CH))
		return do_error(CTC_CH_OUT_OF_RANGE);

	if ((newVal < 1) || (newVal > 6))
		return do_error(CTC_CH_OUT_OF_RANGE);

	if (!do_io(handle,&(map->channels[chan - F_OUT_CH].confChan),&curVal,DO_READ))
		return do_error(CTC_DAL_ERR);

	curVal &= ~(CHCFG_C1S_MASK);  /* cleanout old value */
	curVal |= (((newVal - 1) & CHCFG_C1S_SIGBITS_MASK) << CHCFG_C1S_SHIFT);

	if (!do_io(handle,&(map->channels[chan - F_OUT_CH].confChan),&curVal,DO_WRITE))
		return do_error(CTC_DAL_ERR);

	return OK;
}

/**
 * =====================================================================
 * @brief Returns current clock setting for the second counter.
 *
 * @param handle - DAL handle
 * @param chan   - [1, 8] range
 * @param res    - reg val will be put here
 *
 * @note Real register value will be interpreted here, since the clock setting,
 *       as seen by the user is (reg_value + 1), i.e.\n
 *       0 - clock1\n
 *       1 - clock2 etc...
 *
 * @return OK                   - if succeed. i.e. 0
 * @return -CTC_CH_OUT_OF_RANGE - input/output chan out of range
 * @return -CTC_DAL_ERR         - DAL return error
 */

int ctc_getClock2(HANDLE handle, int chan, unsigned long *res)
{
	int locRes;

	if ((chan < F_OUT_CH) || (chan > L_OUT_CH))
		return do_error(CTC_CH_OUT_OF_RANGE);

	if (!do_io(handle,&(map->channels[chan - F_OUT_CH].confChan),&locRes,DO_READ))
		return do_error(CTC_DAL_ERR);

	locRes = (locRes & CHCFG_C2S_MASK) >> CHCFG_C2S_SHIFT;
	*res = locRes + 1;

	return OK;
}

/**
 * =====================================================================
 * @brief Set external clock for the second counter.
 *
 * @param handle - DAL handle
 * @param chan   - [1, 8] range
 * @param newVal - value to set [1, 6]
 *
 * @note Provided user value will be interpreted, since the the real clock
 *       setting is (user_value - 1), i.e.\n
 *       0 - clock1\n
 *       1 - clock2 etc...
 *
 * @return OK                    - if succeed. i.e. 0
 * @return -CTC_CH_OUT_OF_RANGE  - input/output chan out of range
 * @return -CTC_VAL_OUT_OF_RANGE - requested external clock is out-of-range
 * @return -CTC_DAL_ERR          - DAL return error
 */

int ctc_setClock2(HANDLE handle, int chan, unsigned long newVal)
{
	int curVal;

	if ((chan < F_OUT_CH) || (chan > L_OUT_CH))
		return do_error(CTC_CH_OUT_OF_RANGE);

	if ((newVal < 1) || (newVal > 6))
		return do_error(CTC_CH_OUT_OF_RANGE);

	if (!do_io(handle,&(map->channels[chan - F_OUT_CH].confChan),&curVal,DO_READ))
		return do_error(CTC_DAL_ERR);

	curVal &= ~(CHCFG_C2S_MASK);  /* cleanout old value */
	curVal |= (((newVal - 1) & CHCFG_C2S_SIGBITS_MASK) << CHCFG_C2S_SHIFT);

	if (!do_io(handle,&(map->channels[chan - F_OUT_CH].confChan),&curVal,DO_WRITE))
		return do_error(CTC_DAL_ERR);

	return OK;
}

/**
 * =====================================================================
 * @brief
 *
 * @param handle - DAL handle
 * @param chan   - [1, 8] range
 * @param res    - reg val will be put here
 *
 * @return OK                   - if succeed. i.e. 0
 * @return -CTC_CH_OUT_OF_RANGE - input/output chan out of range
 * @return -CTC_DAL_ERR         - DAL return error
 */

int ctc_getClock1Tick(HANDLE handle, int chan, unsigned long *res)
{
	int locRes;

	if ((chan < F_OUT_CH) || (chan > L_OUT_CH))
		return do_error(CTC_CH_OUT_OF_RANGE);

	if (!do_io(handle,&(map->channels[chan - F_OUT_CH].clock1Delay),&locRes,DO_READ))
		return do_error(CTC_DAL_ERR);

	*res = locRes;
	return OK;
}

/**
 * =====================================================================
 * @brief
 *
 * @param handle - DAL handle
 * @param chan   - [1, 8] range
 * @param newVal - value to set
 *
 * @return OK                   - if succeed. i.e. 0
 * @return -CTC_CH_OUT_OF_RANGE - input/output chan out of range
 * @return -CTC_DAL_ERR         - DAL return error
 */

int ctc_setClock1Tick(HANDLE handle, int chan, unsigned long newVal)
{
	int curVal;

	if ((chan < F_OUT_CH) || (chan > L_OUT_CH))
		return do_error(CTC_CH_OUT_OF_RANGE);

	curVal = 0;
	if (!do_io(handle,&(map->channels[chan - F_OUT_CH].clock1Delay),&curVal,DO_WRITE))
		return do_error(CTC_DAL_ERR);

	curVal = newVal;
	if (!do_io(handle,&(map->channels[chan - F_OUT_CH].clock1Delay),&curVal,DO_WRITE))
		return do_error(CTC_DAL_ERR);

	return OK;
}

/**
 * =====================================================================
 * @brief
 *
 * @param handle - DAL handle
 * @param chan   - [1, 8] range
 * @param res    - reg val will be put here
 *
 * @return OK                   - if succeed. i.e. 0
 * @return -CTC_CH_OUT_OF_RANGE - input/output chan out of range
 * @return -CTC_DAL_ERR         - DAL return error
 */

int ctc_getClock2Tick(HANDLE handle, int chan, unsigned long *res)
{
	int locRes;

	if ((chan < F_OUT_CH) || (chan > L_OUT_CH))
		return do_error(CTC_CH_OUT_OF_RANGE);

	if (!do_io(handle,&(map->channels[chan - F_OUT_CH].clock2Delay),&locRes,DO_READ))
		return do_error(CTC_DAL_ERR);

	*res = locRes;
	return OK;
}

/**
 * =====================================================================
 * @brief
 *
 * @param handle - DAL handle
 * @param chan   - [1, 8] range
 * @param newVal - value to set
 *
 * @return OK                   - if succeed. i.e. 0
 * @return -CTC_CH_OUT_OF_RANGE - input/output chan out of range
 * @return -CTC_DAL_ERR         - DAL return error
 */

int ctc_setClock2Tick(HANDLE handle, int chan, unsigned long newVal)
{
	int curVal;

	if ((chan < F_OUT_CH) || (chan > L_OUT_CH))
		return do_error(CTC_CH_OUT_OF_RANGE);

	curVal = 0;
	if (!do_io(handle,&(map->channels[chan - F_OUT_CH].clock2Delay),&curVal,DO_WRITE))
		return do_error(CTC_DAL_ERR);

	curVal = newVal;
	if (!do_io(handle,&(map->channels[chan - F_OUT_CH].clock2Delay),&curVal,DO_WRITE))
		return do_error(CTC_DAL_ERR);

	return OK;
}

/**
 * =====================================================================
 * @brief
 *
 * @param handle - DAL handle
 * @param chan   - [1, 8] range
 * @param res    - reg val will be put here
 *
 * @return OK                   - if succeed. i.e. 0
 * @return -CTC_CH_OUT_OF_RANGE - input/output chan out of range
 * @return -CTC_DAL_ERR         - DAL return error
 */

int ctc_getOutPutCounter(HANDLE handle, int chan, unsigned long *res)
{
	int locRes;

	if ((chan < F_OUT_CH) || (chan > L_OUT_CH))
		return do_error(CTC_CH_OUT_OF_RANGE);

	if (!do_io(handle,&(map->channels[chan - F_OUT_CH].outputCntr),&locRes,DO_READ))
		return do_error(CTC_DAL_ERR);

	*res = locRes;
	return OK;
}

/**
 * =====================================================================
 * @brief Get current value of the 1'st counter.
 *
 * @param handle - DAL handle
 * @param chan   - [1, 8] range
 * @param res    - reg val will be put here
 *
 * @return OK                   - if succeed. i.e. 0
 * @return -CTC_CH_OUT_OF_RANGE - input/output chan out of range
 * @return -CTC_DAL_ERR         - DAL return error
 */

int ctc_getCntr1CurVal(HANDLE handle, int chan, unsigned long *res)
{
	int locRes;

	if ((chan < F_OUT_CH) || (chan > L_OUT_CH))
		return do_error(CTC_CH_OUT_OF_RANGE);

	if (!do_io(handle,&(map->channels[chan - F_OUT_CH].cntr1CurVal),&locRes,DO_READ))
		return do_error(CTC_DAL_ERR);

	*res = locRes;
	return OK;
}

/**
 * =====================================================================
 * @brief Get current value of the 2'nd counter.
 *
 * @param handle - DAL handle
 * @param chan   - [1, 8] range
 * @param res    - reg val will be put here
 *
 * @return OK                   - if succeed. i.e. 0
 * @return -CTC_CH_OUT_OF_RANGE - input/output chan out of range
 * @return -CTC_DAL_ERR         - DAL return error
 */

int ctc_getCntr2CurVal(HANDLE handle, int chan, unsigned long *res)
{
	int locRes;

	if ((chan < F_OUT_CH) || (chan > L_OUT_CH))
		return do_error(CTC_CH_OUT_OF_RANGE);

	if (!do_io(handle,&(map->channels[chan - F_OUT_CH].cntr2CurVal),&locRes,DO_READ))
		return do_error(CTC_DAL_ERR);

	*res = locRes;
	return OK;
}

/**
 * =====================================================================
 * @brief Obtain module status register (get current Hardware version).
 *
 * @param handle - DAL handle
 * @param res    - reg val will be put here
 *
 * @return OK           - if succeed. i.e. 0
 * @return -CTC_DAL_ERR - DAL return error
 */

int ctc_getModuleStatus(HANDLE handle, unsigned long *res)
{
	int locRes;

	if (!do_io(handle,&(map->status),&locRes,DO_READ))
		return do_error(CTC_DAL_ERR);

	*res = locRes;
	return OK;
}

/*
 * =====================================================================
   RESET register bit layout:

   Only 9 LSB are taken into account [0 - 8].
   First one is to reset the whole module.
   All the rest are to enable/disable channels. bit 1 - channel 8,
   bit 2 - channel 7 ... bit 8 - channel 1

   bit:    8     7     6     5     4     3     2     1        0
        +-----+-----+-----+-----+-----+-----+-----+-----+-----------+
        | ch1 | ch2 | ch3 | ch4 | ch5 | ch6 | ch7 | ch8 | mod reset |
        +-----+-----+-----+-----+-----+-----+-----+-----+-----------+

   To enable the channel  - write 1 to appropriate bit.
   To disable the channel - write 0 to appropriate bit.
   To reset the module    - write 1 to the bit zero.
 */

/**
 * =====================================================================
 * @brief Reset module and disable all the channels.
 *
 * @param handle - DAL handle
 *
 * @return OK           - if succeed. i.e. 0
 * @return -CTC_DAL_ERR - DAL return error
 * @return -CTC_ERR     - ctc lib function error
 */

int ctc_resetModule(HANDLE handle)
{
	int curRegVal;
	int cntr;

	if (!do_io(handle,&(map->cntr_enable),&curRegVal,DO_READ))
		return do_error(CTC_DAL_ERR);

	curRegVal = 1;
	if (!do_io(handle,&(map->cntr_enable),&curRegVal,DO_WRITE))
		return do_error(CTC_DAL_ERR);


	curRegVal = 0; /* change module reset bit to zero and reset all the channels,
			  so that next time we'll write some value into reset
			  register, no module reset will be performed by accident */

	if (!do_io(handle,&(map->cntr_enable),&curRegVal,DO_WRITE))
		return do_error(CTC_DAL_ERR);

	if (!do_io(handle,&(map->cntr_enable),&curRegVal,DO_READ))
		return do_error(CTC_DAL_ERR);

	/* set all channel inputs and counter delays to zero */

	for (cntr = 1; cntr <= 8; cntr++)
		if ( ctc_setInputChan(handle, cntr, 0) ||
		     ctc_setClock1Tick(handle, cntr,0) ||
		     ctc_setClock2Tick(handle, cntr,0) ) {
	      return do_error(CTC_ERR);
	}
       return OK;
}

/**
 * =====================================================================
 * @brief Set input <-> output channel mapping and enable specified
 *        output channel.
 *
 * @param handle  - DAL handle
 * @param outChan - output channel.[1,  8] range
 * @param inpChan - input channel. [1, 40] range
 *
 * @return OK                   - if succeed. i.e. 0
 * @return -CTC_CH_OUT_OF_RANGE - input/output chan out of range
 * @return -CTC_DAL_ERR         - DAL return error
 * @return -CTC_ERR             - ctc lib function error
 */

int ctc_enableChannel(HANDLE handle, int outChan, int inpChan)
{
	int curVal;

	if ((outChan < 1) || (outChan > L_OUT_CH))
		return do_error(CTC_CH_OUT_OF_RANGE);

	if ((inpChan < 1) || (inpChan > L_OUT_CH))
		return do_error(CTC_CH_OUT_OF_RANGE);

	/* set input channel in config register of the channel */

	if (ctc_setInputChan(handle, outChan, inpChan))
		return do_error(CTC_ERR);

	if (!do_io(handle,&(map->cntr_enable),&curVal,DO_READ))
		return do_error(CTC_DAL_ERR);

	curVal |= 1 << (F_OUT_CH + L_OUT_CH - outChan);

	if (!do_io(handle,&(map->cntr_enable),&curVal,DO_WRITE))
		return do_error(CTC_DAL_ERR);

	return OK;
}

/**
 * =====================================================================
 * @brief Disable specified output channel and unset input <-> output channel
 *        mapping.
 *
 * @param handle  - DAL handle
 * @param outChan - output channel to disable. [1, 8] range
 *
 * To disable channel, we should write 0 to the corresponding bit of the reset
 * register. Moreover, current external start (input channel) is set to zero.
 *
 * @return OK                   - if succeed. i.e. 0
 * @return -CTC_CH_OUT_OF_RANGE - input/output chan out of range
 * @return -CTC_DAL_ERR         - DAL return error
 */

int ctc_disableChannel(HANDLE handle, int outChan)
{
	int curVal;

	if ((outChan < 1) || (outChan > L_OUT_CH))
		return do_error(CTC_CH_OUT_OF_RANGE);

	if (!do_io(handle,&(map->cntr_enable),&curVal,DO_READ))
		return do_error(CTC_DAL_ERR);

	curVal &= ~(1 << (F_OUT_CH + L_OUT_CH - outChan));

	if (!do_io(handle,&(map->cntr_enable),&curVal,DO_WRITE))
		return do_error(CTC_DAL_ERR);

	return OK;
}

/**
 * =====================================================================
 * @brief To get channel configuration data.
 *
 * @param handle  - DAL handle
 * @param outChan - Junk argument
 *
 * @return OK                   - if succeed. i.e. 0
 * @return -CTC_CH_OUT_OF_RANGE - input/output chan out of range
 * @return -CTC_DAL_ERR         - DAL return error
 */

ctc_cfg_reg_t *ctc_getChanConf(HANDLE handle, int outChan)
{
	static ctc_cfg_reg_t ctc_conf_data[L_OUT_CH]; /* Horrible, not thread safe */
	int curVal, cntr;

	for (cntr = 0; cntr < L_OUT_CH; cntr++) {
		if (!do_io(handle,&(map->channels[cntr].confChan),&curVal,DO_WRITE))
			return (ctc_cfg_reg_t *) do_error(CTC_DAL_ERR);
		ctc_conf_data[cntr].cr_ext_start = ((curVal & CHCFG_ESC_MASK) >> CHCFG_ESC_SHIFT) + 1;
		ctc_conf_data[cntr].cr_cntr1_clk = ((curVal & CHCFG_C1S_MASK) >> CHCFG_C1S_SHIFT) + 1;
		ctc_conf_data[cntr].cr_cntr2_clk = ((curVal & CHCFG_C2S_MASK) >> CHCFG_C2S_SHIFT) + 1;
		ctc_conf_data[cntr].cr_mode      =   curVal & CHCFG_MOD_MASK;
		ctc_conf_data[cntr].cr_direction =   curVal & CHCFG_DIR_MASK;
	}
       return ctc_conf_data;
}

/**
 * =====================================================================
 * @brief Enable library debug printout and toggle debug flag.
 *
 * @param toggle - if 1 - will toggle current flag and return the previous
 *                 flag value.\n
 *                 if 0 - just give current value without flag toggling.
 *
 * @return previous/current debug flag
 */

int ctc_dbgPrintout(int toggle)
{
	int old_flg;

	old_flg = ctc_lib_glob_dbg_flag;

	if (toggle)
	    ctc_lib_glob_dbg_flag ^= 1;

	return old_flg;
}

/**
 * =====================================================================
 * ctc_getChannelStatus - Get current status of 8 output channels.
 *
 * @param handle  - DAL handle
 * @param outChan - massive to store current status in. Must be 8 integers long.
 *
 * Output channel can be either enabled or disabled.\n
 * Each value in @outChan represents current status of the output channel.\n
 * (outChan[0] - for channel 1, outChan[7] - for channel 8, etc...)\n
 * If channel value is 0 - output channel is disabled.\n
 * If channle value is one of [1 - 40] - output channel is active and
 * connected to the corresponding input channel.
 *
 * @return OK           - all OK.
 * @return -CTC_DAL_ERR - DAL return error.
 */

int ctc_getChannelStatus(HANDLE handle, int outChan[8])
{
	ctc_cfg_reg_t *data_p = NULL;
	int count_enb_reg;
	int cntr;

	if (!do_io(handle,&(map->cntr_enable),&count_enb_reg,DO_READ))
		return do_error(CTC_DAL_ERR);

	for (cntr = 0; cntr < L_OUT_CH; cntr++)
		outChan[cntr] =
			(count_enb_reg & (1 << (L_OUT_CH - cntr))) >>
			(L_OUT_CH - cntr);

	data_p = ctc_getChanConf(handle, 0 /*get all the channels*/ );
	if (data_p < 0)
		return (int) data_p;

	for (cntr = 0; cntr < L_OUT_CH; cntr++)
		if (outChan[cntr])
			outChan[cntr] = data_p[cntr].cr_ext_start;

	return OK;
}

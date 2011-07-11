#include <sys/ioctl.h>
#include <errno.h>
#include "vmeio.h"
#include "rfl4_switch_lim_regs.h"

enum encore_direction {
	READ,
	WRITE,
};

static int reg_wnum(struct encore_reginfo *reg)
{
	char sz = reg->wordsize[0];

	switch (sz) {
	case 'c':
		return 8;
	case 's':
		return 16;
	case 'l':
		return 32;
	default:
		return 0;
	}
}

static int get_set_register(int fd, 
	struct encore_reginfo *reg, void *buffer,
	enum encore_direction direction)
{
	struct vmeio_riob riob, *riobp = &riob;
	int wordsize = reg_wnum(reg);

	riobp->mapnum = reg->block_address_space;
	riobp->offset = reg->offset;
	riobp->wsize  = reg->depth;
	riobp->buffer = buffer;
	riobp->data_width = wordsize;

	if (direction == WRITE)
		return ioctl(fd, VMEIO_RAW_WRITE, riobp);
	else if (direction == READ)
		return ioctl(fd, VMEIO_RAW_READ, riobp);
	else
		return -EINVAL;
}

static int get_set_window(int fd, 
	struct encore_reginfo *reg,
	void *buffer, int from, int to, 
	enum encore_direction direction)
{
	struct vmeio_riob riob, *riobp = &riob;
	int wordsize = reg_wnum(reg);
	int bytesize = wordsize/8;

	riobp->mapnum = reg->block_address_space;
	riobp->offset = reg->offset + from * bytesize;
	riobp->wsize  = to-from;
	riobp->buffer = buffer;
	riobp->data_width = wordsize;

	if (direction == WRITE)
		return ioctl(fd, VMEIO_RAW_WRITE, riobp);
	else if (direction == READ)
		return ioctl(fd, VMEIO_RAW_READ, riobp);
	else
		return -EINVAL;
}


int rfl4_switch_lim_get_control1(int fd, short *buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[control1];
	return get_set_register(fd, reg, buf, READ);
}

int rfl4_switch_lim_set_control1(int fd, short buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[control1];
	return get_set_register(fd, reg, &buf, WRITE);
}

int rfl4_switch_lim_get_vmeIRQStatID(int fd, short *buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[vmeIRQStatID];
	return get_set_register(fd, reg, buf, READ);
}

int rfl4_switch_lim_set_vmeIRQStatID(int fd, short buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[vmeIRQStatID];
	return get_set_register(fd, reg, &buf, WRITE);
}

int rfl4_switch_lim_get_vmeIRQLevel(int fd, short *buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[vmeIRQLevel];
	return get_set_register(fd, reg, buf, READ);
}

int rfl4_switch_lim_set_vmeIRQLevel(int fd, short buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[vmeIRQLevel];
	return get_set_register(fd, reg, &buf, WRITE);
}

int rfl4_switch_lim_get_status1(int fd, short *buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[status1];
	return get_set_register(fd, reg, buf, READ);
}

int rfl4_switch_lim_get_ident(int fd, short *buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[ident];
	return get_set_register(fd, reg, buf, READ);
}

int rfl4_switch_lim_get_faults1(int fd, short *buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[faults1];
	return get_set_register(fd, reg, buf, READ);
}

int rfl4_switch_lim_get_overflow1(int fd, short *buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[overflow1];
	return get_set_register(fd, reg, buf, READ);
}

int rfl4_switch_lim_get_pmAddr(int fd, long *buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[pmAddr];
	return get_set_register(fd, reg, buf, READ);
}

int rfl4_switch_lim_get_obsAddr(int fd, long *buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[obsAddr];
	return get_set_register(fd, reg, buf, READ);
}

int rfl4_switch_lim_get_memStatus(int fd, short *buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[memStatus];
	return get_set_register(fd, reg, buf, READ);
}

int rfl4_switch_lim_get_memControl1(int fd, short *buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[memControl1];
	return get_set_register(fd, reg, buf, READ);
}

int rfl4_switch_lim_set_memControl1(int fd, short buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[memControl1];
	return get_set_register(fd, reg, &buf, WRITE);
}

int rfl4_switch_lim_get_memControl2(int fd, short *buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[memControl2];
	return get_set_register(fd, reg, buf, READ);
}

int rfl4_switch_lim_set_memControl2(int fd, short buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[memControl2];
	return get_set_register(fd, reg, &buf, WRITE);
}

int rfl4_switch_lim_get_memControl3(int fd, short *buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[memControl3];
	return get_set_register(fd, reg, buf, READ);
}

int rfl4_switch_lim_set_memControl3(int fd, short buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[memControl3];
	return get_set_register(fd, reg, &buf, WRITE);
}

int rfl4_switch_lim_get_nvMemStatus(int fd, short *buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[nvMemStatus];
	return get_set_register(fd, reg, buf, READ);
}

int rfl4_switch_lim_get_nvMemControl(int fd, short *buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[nvMemControl];
	return get_set_register(fd, reg, buf, READ);
}

int rfl4_switch_lim_set_nvMemControl(int fd, short buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[nvMemControl];
	return get_set_register(fd, reg, &buf, WRITE);
}

int rfl4_switch_lim_get_nvMemGetKey(int fd, short *buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[nvMemGetKey];
	return get_set_register(fd, reg, buf, READ);
}

int rfl4_switch_lim_set_nvMemSetKey(int fd, short buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[nvMemSetKey];
	return get_set_register(fd, reg, &buf, WRITE);
}

int rfl4_switch_lim_get_nvMemCRCVal(int fd, short *buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[nvMemCRCVal];
	return get_set_register(fd, reg, buf, READ);
}

int rfl4_switch_lim_get_rfLimitingLevel(int fd, short *buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[rfLimitingLevel];
	return get_set_register(fd, reg, buf, READ);
}

int rfl4_switch_lim_set_rfLimitingLevel(int fd, short buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[rfLimitingLevel];
	return get_set_register(fd, reg, &buf, WRITE);
}

int rfl4_switch_lim_get_rfGainLevel(int fd, short *buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[rfGainLevel];
	return get_set_register(fd, reg, buf, READ);
}

int rfl4_switch_lim_set_rfGainLevel(int fd, short buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[rfGainLevel];
	return get_set_register(fd, reg, &buf, WRITE);
}

int rfl4_switch_lim_get_fbSetPoint(int fd, short *buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[fbSetPoint];
	return get_set_register(fd, reg, buf, READ);
}

int rfl4_switch_lim_set_fbSetPoint(int fd, short buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[fbSetPoint];
	return get_set_register(fd, reg, &buf, WRITE);
}

int rfl4_switch_lim_get_rfLimitingDelay(int fd, short *buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[rfLimitingDelay];
	return get_set_register(fd, reg, buf, READ);
}

int rfl4_switch_lim_set_rfLimitingDelay(int fd, short buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[rfLimitingDelay];
	return get_set_register(fd, reg, &buf, WRITE);
}

int rfl4_switch_lim_get_loPwrMax(int fd, short *buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[loPwrMax];
	return get_set_register(fd, reg, buf, READ);
}

int rfl4_switch_lim_set_loPwrMax(int fd, short buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[loPwrMax];
	return get_set_register(fd, reg, &buf, WRITE);
}

int rfl4_switch_lim_get_loPwrMin(int fd, short *buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[loPwrMin];
	return get_set_register(fd, reg, buf, READ);
}

int rfl4_switch_lim_set_loPwrMin(int fd, short buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[loPwrMin];
	return get_set_register(fd, reg, &buf, WRITE);
}

int rfl4_switch_lim_get_inputsControl(int fd, short *buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[inputsControl];
	return get_set_register(fd, reg, buf, READ);
}

int rfl4_switch_lim_set_inputsControl(int fd, short buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[inputsControl];
	return get_set_register(fd, reg, &buf, WRITE);
}

int rfl4_switch_lim_get_irqControl(int fd, short *buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[irqControl];
	return get_set_register(fd, reg, buf, READ);
}

int rfl4_switch_lim_set_irqControl(int fd, short buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[irqControl];
	return get_set_register(fd, reg, &buf, WRITE);
}

int rfl4_switch_lim_get_vetoStatus(int fd, short *buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[vetoStatus];
	return get_set_register(fd, reg, buf, READ);
}

int rfl4_switch_lim_get_vetoControl(int fd, short *buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[vetoControl];
	return get_set_register(fd, reg, buf, READ);
}

int rfl4_switch_lim_set_vetoControl(int fd, short buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[vetoControl];
	return get_set_register(fd, reg, &buf, WRITE);
}

int rfl4_switch_lim_get_status2(int fd, short *buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[status2];
	return get_set_register(fd, reg, buf, READ);
}

int rfl4_switch_lim_get_clockStatus(int fd, short *buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[clockStatus];
	return get_set_register(fd, reg, buf, READ);
}

int rfl4_switch_lim_get_diagInPwr(int fd, short *buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[diagInPwr];
	return get_set_register(fd, reg, buf, READ);
}

int rfl4_switch_lim_get_diagOutPwr(int fd, short *buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[diagOutPwr];
	return get_set_register(fd, reg, buf, READ);
}

int rfl4_switch_lim_get_diagInPhase(int fd, short *buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[diagInPhase];
	return get_set_register(fd, reg, buf, READ);
}

int rfl4_switch_lim_get_diagOutPhase(int fd, short *buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[diagOutPhase];
	return get_set_register(fd, reg, buf, READ);
}

int rfl4_switch_lim_get_frevAdjDiagIn(int fd, short *buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[frevAdjDiagIn];
	return get_set_register(fd, reg, buf, READ);
}

int rfl4_switch_lim_set_frevAdjDiagIn(int fd, short buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[frevAdjDiagIn];
	return get_set_register(fd, reg, &buf, WRITE);
}

int rfl4_switch_lim_get_frevAdjDiagOut(int fd, short *buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[frevAdjDiagOut];
	return get_set_register(fd, reg, buf, READ);
}

int rfl4_switch_lim_set_frevAdjDiagOut(int fd, short buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[frevAdjDiagOut];
	return get_set_register(fd, reg, &buf, WRITE);
}

int rfl4_switch_lim_get_frevAdjH(int fd, short *buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[frevAdjH];
	return get_set_register(fd, reg, buf, READ);
}

int rfl4_switch_lim_set_frevAdjH(int fd, short buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[frevAdjH];
	return get_set_register(fd, reg, &buf, WRITE);
}

int rfl4_switch_lim_get_adcTestControl(int fd, short *buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[adcTestControl];
	return get_set_register(fd, reg, buf, READ);
}

int rfl4_switch_lim_set_adcTestControl(int fd, short buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[adcTestControl];
	return get_set_register(fd, reg, &buf, WRITE);
}

int rfl4_switch_lim_get_adcTestStatus(int fd, short *buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[adcTestStatus];
	return get_set_register(fd, reg, buf, READ);
}

int rfl4_switch_lim_get_dac1Cal(int fd, short *buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[dac1Cal];
	return get_set_register(fd, reg, buf, READ);
}

int rfl4_switch_lim_get_dac2Cal(int fd, short *buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[dac2Cal];
	return get_set_register(fd, reg, buf, READ);
}

int rfl4_switch_lim_get_dac3Cal(int fd, short *buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[dac3Cal];
	return get_set_register(fd, reg, buf, READ);
}

int rfl4_switch_lim_get_adclkFreqMax(int fd, short *buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[adclkFreqMax];
	return get_set_register(fd, reg, buf, READ);
}

int rfl4_switch_lim_set_adclkFreqMax(int fd, short buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[adclkFreqMax];
	return get_set_register(fd, reg, &buf, WRITE);
}

int rfl4_switch_lim_get_adclkFreqMin(int fd, short *buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[adclkFreqMin];
	return get_set_register(fd, reg, buf, READ);
}

int rfl4_switch_lim_set_adclkFreqMin(int fd, short buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[adclkFreqMin];
	return get_set_register(fd, reg, &buf, WRITE);
}

int rfl4_switch_lim_get_frevDetector(int fd, short *buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[frevDetector];
	return get_set_register(fd, reg, buf, READ);
}

int rfl4_switch_lim_set_frevDetector(int fd, short buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[frevDetector];
	return get_set_register(fd, reg, &buf, WRITE);
}

int rfl4_switch_lim_get_fcDetector(int fd, short *buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[fcDetector];
	return get_set_register(fd, reg, buf, READ);
}

int rfl4_switch_lim_set_fcDetector(int fd, short buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[fcDetector];
	return get_set_register(fd, reg, &buf, WRITE);
}

int rfl4_switch_lim_get_loPwr(int fd, short *buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[loPwr];
	return get_set_register(fd, reg, buf, READ);
}

int rfl4_switch_lim_get_uP1V2(int fd, short *buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[uP1V2];
	return get_set_register(fd, reg, buf, READ);
}

int rfl4_switch_lim_get_uP1V8(int fd, short *buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[uP1V8];
	return get_set_register(fd, reg, buf, READ);
}

int rfl4_switch_lim_get_uP2V5(int fd, short *buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[uP2V5];
	return get_set_register(fd, reg, buf, READ);
}

int rfl4_switch_lim_get_uP3V3(int fd, short *buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[uP3V3];
	return get_set_register(fd, reg, buf, READ);
}

int rfl4_switch_lim_get_uP1V8A(int fd, short *buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[uP1V8A];
	return get_set_register(fd, reg, buf, READ);
}

int rfl4_switch_lim_get_uP5VA(int fd, short *buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[uP5VA];
	return get_set_register(fd, reg, buf, READ);
}

int rfl4_switch_lim_get_uP8V(int fd, short *buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[uP8V];
	return get_set_register(fd, reg, buf, READ);
}

int rfl4_switch_lim_get_uM3V3(int fd, short *buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[uM3V3];
	return get_set_register(fd, reg, buf, READ);
}

int rfl4_switch_lim_get_uM5VA(int fd, short *buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[uM5VA];
	return get_set_register(fd, reg, buf, READ);
}

int rfl4_switch_lim_get_uP3V3D(int fd, short *buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[uP3V3D];
	return get_set_register(fd, reg, buf, READ);
}

int rfl4_switch_lim_get_uP5V(int fd, short *buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[uP5V];
	return get_set_register(fd, reg, buf, READ);
}

int rfl4_switch_lim_get_uP12V(int fd, short *buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[uP12V];
	return get_set_register(fd, reg, buf, READ);
}

int rfl4_switch_lim_get_uM12V(int fd, short *buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[uM12V];
	return get_set_register(fd, reg, buf, READ);
}

int rfl4_switch_lim_get_uP12VA(int fd, short *buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[uP12VA];
	return get_set_register(fd, reg, buf, READ);
}

int rfl4_switch_lim_get_uM12VA(int fd, short *buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[uM12VA];
	return get_set_register(fd, reg, buf, READ);
}

int rfl4_switch_lim_get_uP6VA(int fd, short *buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[uP6VA];
	return get_set_register(fd, reg, buf, READ);
}

int rfl4_switch_lim_get_uM6VA(int fd, short *buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[uM6VA];
	return get_set_register(fd, reg, buf, READ);
}

int rfl4_switch_lim_get_obsEvntCycleTime(int fd, long *buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[obsEvntCycleTime];
	return get_set_register(fd, reg, buf, READ);
}

int rfl4_switch_lim_get_pmEvntCycleTime(int fd, long *buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[pmEvntCycleTime];
	return get_set_register(fd, reg, buf, READ);
}

int rfl4_switch_lim_get_cycleTimeControl(int fd, short *buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[cycleTimeControl];
	return get_set_register(fd, reg, buf, READ);
}

int rfl4_switch_lim_set_cycleTimeControl(int fd, short buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[cycleTimeControl];
	return get_set_register(fd, reg, &buf, WRITE);
}

int rfl4_switch_lim_get_timingSel1(int fd, short *buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[timingSel1];
	return get_set_register(fd, reg, buf, READ);
}

int rfl4_switch_lim_set_timingSel1(int fd, short buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[timingSel1];
	return get_set_register(fd, reg, &buf, WRITE);
}

int rfl4_switch_lim_get_refPhase(int fd, short *buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[refPhase];
	return get_set_register(fd, reg, buf, READ);
}

int rfl4_switch_lim_get_pmObsFrzTrigger(int fd, short *buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[pmObsFrzTrigger];
	return get_set_register(fd, reg, buf, READ);
}

int rfl4_switch_lim_set_pmObsFrzTrigger(int fd, short buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[pmObsFrzTrigger];
	return get_set_register(fd, reg, &buf, WRITE);
}

int rfl4_switch_lim_get_rfFreq(int fd, short *buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[rfFreq];
	return get_set_register(fd, reg, buf, READ);
}

int rfl4_switch_lim_get_rfFreqMax(int fd, short *buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[rfFreqMax];
	return get_set_register(fd, reg, buf, READ);
}

int rfl4_switch_lim_set_rfFreqMax(int fd, short buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[rfFreqMax];
	return get_set_register(fd, reg, &buf, WRITE);
}

int rfl4_switch_lim_get_rfFreqMin(int fd, short *buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[rfFreqMin];
	return get_set_register(fd, reg, buf, READ);
}

int rfl4_switch_lim_set_rfFreqMin(int fd, short buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[rfFreqMin];
	return get_set_register(fd, reg, &buf, WRITE);
}

int rfl4_switch_lim_get_fcAdjH(int fd, short *buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[fcAdjH];
	return get_set_register(fd, reg, buf, READ);
}

int rfl4_switch_lim_set_fcAdjH(int fd, short buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[fcAdjH];
	return get_set_register(fd, reg, &buf, WRITE);
}

int rfl4_switch_lim_get_adcTestData(int fd, short *buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[adcTestData];
	return get_set_register(fd, reg, buf, READ);
}

int rfl4_switch_lim_set_adcTestData(int fd, short buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[adcTestData];
	return get_set_register(fd, reg, &buf, WRITE);
}

int rfl4_switch_lim_get_sysControl(int fd, short *buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[sysControl];
	return get_set_register(fd, reg, buf, READ);
}

int rfl4_switch_lim_set_sysControl(int fd, short buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[sysControl];
	return get_set_register(fd, reg, &buf, WRITE);
}

int rfl4_switch_lim_get_hardwareVersion(int fd, short *buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[hardwareVersion];
	return get_set_register(fd, reg, buf, READ);
}

int rfl4_switch_lim_get_designerID(int fd, short *buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[designerID];
	return get_set_register(fd, reg, buf, READ);
}

int rfl4_switch_lim_get_boardTemp(int fd, short *buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[boardTemp];
	return get_set_register(fd, reg, buf, READ);
}

int rfl4_switch_lim_get_chipTemp(int fd, short *buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[chipTemp];
	return get_set_register(fd, reg, buf, READ);
}

int rfl4_switch_lim_get_testControl(int fd, short *buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[testControl];
	return get_set_register(fd, reg, buf, READ);
}

int rfl4_switch_lim_set_testControl(int fd, short buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[testControl];
	return get_set_register(fd, reg, &buf, WRITE);
}

int rfl4_switch_lim_get_testStatus(int fd, short *buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[testStatus];
	return get_set_register(fd, reg, buf, READ);
}

int rfl4_switch_lim_get_firmwareVersion(int fd, long *buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[firmwareVersion];
	return get_set_register(fd, reg, buf, READ);
}

int rfl4_switch_lim_get_serialNumber(int fd, long *buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[serialNumber];
	return get_set_register(fd, reg, buf, READ);
}

int rfl4_switch_lim_get_serialNumber_window(int fd, long buf[], int from, int to)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[serialNumber];
	return get_set_window(fd, reg, buf, from, to, READ);
}

int rfl4_switch_lim_get_memMapVersion(int fd, long *buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[memMapVersion];
	return get_set_register(fd, reg, buf, READ);
}

int rfl4_switch_lim_get_diagInGain(int fd, short *buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[diagInGain];
	return get_set_register(fd, reg, buf, READ);
}

int rfl4_switch_lim_set_diagInGain(int fd, short buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[diagInGain];
	return get_set_register(fd, reg, &buf, WRITE);
}

int rfl4_switch_lim_get_diagInOffset(int fd, short *buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[diagInOffset];
	return get_set_register(fd, reg, buf, READ);
}

int rfl4_switch_lim_set_diagInOffset(int fd, short buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[diagInOffset];
	return get_set_register(fd, reg, &buf, WRITE);
}

int rfl4_switch_lim_get_diagOutGain(int fd, short *buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[diagOutGain];
	return get_set_register(fd, reg, buf, READ);
}

int rfl4_switch_lim_set_diagOutGain(int fd, short buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[diagOutGain];
	return get_set_register(fd, reg, &buf, WRITE);
}

int rfl4_switch_lim_get_diagOutOffset(int fd, short *buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[diagOutOffset];
	return get_set_register(fd, reg, buf, READ);
}

int rfl4_switch_lim_set_diagOutOffset(int fd, short buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[diagOutOffset];
	return get_set_register(fd, reg, &buf, WRITE);
}

int rfl4_switch_lim_get_dac1Gain(int fd, short *buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[dac1Gain];
	return get_set_register(fd, reg, buf, READ);
}

int rfl4_switch_lim_set_dac1Gain(int fd, short buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[dac1Gain];
	return get_set_register(fd, reg, &buf, WRITE);
}

int rfl4_switch_lim_get_dac1Offset(int fd, short *buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[dac1Offset];
	return get_set_register(fd, reg, buf, READ);
}

int rfl4_switch_lim_set_dac1Offset(int fd, short buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[dac1Offset];
	return get_set_register(fd, reg, &buf, WRITE);
}

int rfl4_switch_lim_get_dac2Gain(int fd, short *buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[dac2Gain];
	return get_set_register(fd, reg, buf, READ);
}

int rfl4_switch_lim_set_dac2Gain(int fd, short buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[dac2Gain];
	return get_set_register(fd, reg, &buf, WRITE);
}

int rfl4_switch_lim_get_dac2Offset(int fd, short *buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[dac2Offset];
	return get_set_register(fd, reg, buf, READ);
}

int rfl4_switch_lim_set_dac2Offset(int fd, short buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[dac2Offset];
	return get_set_register(fd, reg, &buf, WRITE);
}

int rfl4_switch_lim_get_dac3Gain(int fd, short *buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[dac3Gain];
	return get_set_register(fd, reg, buf, READ);
}

int rfl4_switch_lim_set_dac3Gain(int fd, short buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[dac3Gain];
	return get_set_register(fd, reg, &buf, WRITE);
}

int rfl4_switch_lim_get_dac3Offset(int fd, short *buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[dac3Offset];
	return get_set_register(fd, reg, buf, READ);
}

int rfl4_switch_lim_set_dac3Offset(int fd, short buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[dac3Offset];
	return get_set_register(fd, reg, &buf, WRITE);
}

int rfl4_switch_lim_get_dac1Gain2(int fd, short *buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[dac1Gain2];
	return get_set_register(fd, reg, buf, READ);
}

int rfl4_switch_lim_set_dac1Gain2(int fd, short buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[dac1Gain2];
	return get_set_register(fd, reg, &buf, WRITE);
}

int rfl4_switch_lim_get_dac1Offset2(int fd, short *buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[dac1Offset2];
	return get_set_register(fd, reg, buf, READ);
}

int rfl4_switch_lim_set_dac1Offset2(int fd, short buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[dac1Offset2];
	return get_set_register(fd, reg, &buf, WRITE);
}

int rfl4_switch_lim_get_powerDetVTAdj(int fd, short *buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[powerDetVTAdj];
	return get_set_register(fd, reg, buf, READ);
}

int rfl4_switch_lim_set_powerDetVTAdj(int fd, short buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[powerDetVTAdj];
	return get_set_register(fd, reg, &buf, WRITE);
}

int rfl4_switch_lim_get_testMem(int fd, short *buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[testMem];
	return get_set_register(fd, reg, buf, READ);
}

int rfl4_switch_lim_get_testMem_window(int fd, short buf[], int from, int to)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[testMem];
	return get_set_window(fd, reg, buf, from, to, READ);
}

int rfl4_switch_lim_set_testMem(int fd, short buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[testMem];
	return get_set_register(fd, reg, &buf, WRITE);
}

int rfl4_switch_lim_set_testMem_window(int fd, short buf[], int from, int to)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[testMem];
	return get_set_window(fd, reg, buf, from, to, WRITE);
}

int rfl4_switch_lim_get_VECTOR(int fd, short *buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[VECTOR];
	return get_set_register(fd, reg, buf, READ);
}

int rfl4_switch_lim_set_VECTOR(int fd, short buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[VECTOR];
	return get_set_register(fd, reg, &buf, WRITE);
}

int rfl4_switch_lim_get_LEVEL(int fd, short *buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[LEVEL];
	return get_set_register(fd, reg, buf, READ);
}

int rfl4_switch_lim_set_LEVEL(int fd, short buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[LEVEL];
	return get_set_register(fd, reg, &buf, WRITE);
}

int rfl4_switch_lim_get_memViewport(int fd, short *buf)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[memViewport];
	return get_set_register(fd, reg, buf, READ);
}

int rfl4_switch_lim_get_memViewport_window(int fd, short buf[], int from, int to)
{
	struct encore_reginfo *reg = &rfl4_switch_lim_registers[memViewport];
	return get_set_window(fd, reg, buf, from, to, READ);
}

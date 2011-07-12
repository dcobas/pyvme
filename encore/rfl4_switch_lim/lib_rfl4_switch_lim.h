/* lib_rfl4_switch_lim API */

#ifndef _LIB_RFL4_SWITCH_LIM_H
#define _LIB_RFL4_SWITCH_LIM_H

#ifdef __cplusplus
extern "C" {
#endif

/* rfl4_switch_lim lib declarations */

int rfl4_switch_lim_open(int lun);
int rfl4_switch_lim_close(int fd);
int rfl4_switch_lim_raw(int fd, int mapping, 
	unsigned offset, unsigned items, unsigned data_width,
	void *buffer, int write);

int rfl4_switch_lim_wait(int fd);
int rfl4_switch_lim_get_control1(int fd, short *buf);
int rfl4_switch_lim_set_control1(int fd, short buf);
int rfl4_switch_lim_get_vmeIRQStatID(int fd, short *buf);
int rfl4_switch_lim_set_vmeIRQStatID(int fd, short buf);
int rfl4_switch_lim_get_vmeIRQLevel(int fd, short *buf);
int rfl4_switch_lim_set_vmeIRQLevel(int fd, short buf);
int rfl4_switch_lim_get_status1(int fd, short *buf);
int rfl4_switch_lim_get_ident(int fd, short *buf);
int rfl4_switch_lim_get_faults1(int fd, short *buf);
int rfl4_switch_lim_get_overflow1(int fd, short *buf);
int rfl4_switch_lim_get_pmAddr(int fd, long *buf);
int rfl4_switch_lim_get_obsAddr(int fd, long *buf);
int rfl4_switch_lim_get_memStatus(int fd, short *buf);
int rfl4_switch_lim_get_memControl1(int fd, short *buf);
int rfl4_switch_lim_set_memControl1(int fd, short buf);
int rfl4_switch_lim_get_memControl2(int fd, short *buf);
int rfl4_switch_lim_set_memControl2(int fd, short buf);
int rfl4_switch_lim_get_memControl3(int fd, short *buf);
int rfl4_switch_lim_set_memControl3(int fd, short buf);
int rfl4_switch_lim_get_nvMemStatus(int fd, short *buf);
int rfl4_switch_lim_get_nvMemControl(int fd, short *buf);
int rfl4_switch_lim_set_nvMemControl(int fd, short buf);
int rfl4_switch_lim_get_nvMemGetKey(int fd, short *buf);
int rfl4_switch_lim_set_nvMemSetKey(int fd, short buf);
int rfl4_switch_lim_get_nvMemCRCVal(int fd, short *buf);
int rfl4_switch_lim_get_rfLimitingLevel(int fd, short *buf);
int rfl4_switch_lim_set_rfLimitingLevel(int fd, short buf);
int rfl4_switch_lim_get_rfGainLevel(int fd, short *buf);
int rfl4_switch_lim_set_rfGainLevel(int fd, short buf);
int rfl4_switch_lim_get_fbSetPoint(int fd, short *buf);
int rfl4_switch_lim_set_fbSetPoint(int fd, short buf);
int rfl4_switch_lim_get_rfLimitingDelay(int fd, short *buf);
int rfl4_switch_lim_set_rfLimitingDelay(int fd, short buf);
int rfl4_switch_lim_get_loPwrMax(int fd, short *buf);
int rfl4_switch_lim_set_loPwrMax(int fd, short buf);
int rfl4_switch_lim_get_loPwrMin(int fd, short *buf);
int rfl4_switch_lim_set_loPwrMin(int fd, short buf);
int rfl4_switch_lim_get_inputsControl(int fd, short *buf);
int rfl4_switch_lim_set_inputsControl(int fd, short buf);
int rfl4_switch_lim_get_irqControl(int fd, short *buf);
int rfl4_switch_lim_set_irqControl(int fd, short buf);
int rfl4_switch_lim_get_vetoStatus(int fd, short *buf);
int rfl4_switch_lim_get_vetoControl(int fd, short *buf);
int rfl4_switch_lim_set_vetoControl(int fd, short buf);
int rfl4_switch_lim_get_status2(int fd, short *buf);
int rfl4_switch_lim_get_clockStatus(int fd, short *buf);
int rfl4_switch_lim_get_diagInPwr(int fd, short *buf);
int rfl4_switch_lim_get_diagOutPwr(int fd, short *buf);
int rfl4_switch_lim_get_diagInPhase(int fd, short *buf);
int rfl4_switch_lim_get_diagOutPhase(int fd, short *buf);
int rfl4_switch_lim_get_frevAdjDiagIn(int fd, short *buf);
int rfl4_switch_lim_set_frevAdjDiagIn(int fd, short buf);
int rfl4_switch_lim_get_frevAdjDiagOut(int fd, short *buf);
int rfl4_switch_lim_set_frevAdjDiagOut(int fd, short buf);
int rfl4_switch_lim_get_frevAdjH(int fd, short *buf);
int rfl4_switch_lim_set_frevAdjH(int fd, short buf);
int rfl4_switch_lim_get_adcTestControl(int fd, short *buf);
int rfl4_switch_lim_set_adcTestControl(int fd, short buf);
int rfl4_switch_lim_get_adcTestStatus(int fd, short *buf);
int rfl4_switch_lim_get_dac1Cal(int fd, short *buf);
int rfl4_switch_lim_get_dac2Cal(int fd, short *buf);
int rfl4_switch_lim_get_dac3Cal(int fd, short *buf);
int rfl4_switch_lim_get_adclkFreqMax(int fd, short *buf);
int rfl4_switch_lim_set_adclkFreqMax(int fd, short buf);
int rfl4_switch_lim_get_adclkFreqMin(int fd, short *buf);
int rfl4_switch_lim_set_adclkFreqMin(int fd, short buf);
int rfl4_switch_lim_get_frevDetector(int fd, short *buf);
int rfl4_switch_lim_set_frevDetector(int fd, short buf);
int rfl4_switch_lim_get_fcDetector(int fd, short *buf);
int rfl4_switch_lim_set_fcDetector(int fd, short buf);
int rfl4_switch_lim_get_loPwr(int fd, short *buf);
int rfl4_switch_lim_get_uP1V2(int fd, short *buf);
int rfl4_switch_lim_get_uP1V8(int fd, short *buf);
int rfl4_switch_lim_get_uP2V5(int fd, short *buf);
int rfl4_switch_lim_get_uP3V3(int fd, short *buf);
int rfl4_switch_lim_get_uP1V8A(int fd, short *buf);
int rfl4_switch_lim_get_uP5VA(int fd, short *buf);
int rfl4_switch_lim_get_uP8V(int fd, short *buf);
int rfl4_switch_lim_get_uM3V3(int fd, short *buf);
int rfl4_switch_lim_get_uM5VA(int fd, short *buf);
int rfl4_switch_lim_get_uP3V3D(int fd, short *buf);
int rfl4_switch_lim_get_uP5V(int fd, short *buf);
int rfl4_switch_lim_get_uP12V(int fd, short *buf);
int rfl4_switch_lim_get_uM12V(int fd, short *buf);
int rfl4_switch_lim_get_uP12VA(int fd, short *buf);
int rfl4_switch_lim_get_uM12VA(int fd, short *buf);
int rfl4_switch_lim_get_uP6VA(int fd, short *buf);
int rfl4_switch_lim_get_uM6VA(int fd, short *buf);
int rfl4_switch_lim_get_obsEvntCycleTime(int fd, long *buf);
int rfl4_switch_lim_get_pmEvntCycleTime(int fd, long *buf);
int rfl4_switch_lim_get_cycleTimeControl(int fd, short *buf);
int rfl4_switch_lim_set_cycleTimeControl(int fd, short buf);
int rfl4_switch_lim_get_timingSel1(int fd, short *buf);
int rfl4_switch_lim_set_timingSel1(int fd, short buf);
int rfl4_switch_lim_get_refPhase(int fd, short *buf);
int rfl4_switch_lim_get_pmObsFrzTrigger(int fd, short *buf);
int rfl4_switch_lim_set_pmObsFrzTrigger(int fd, short buf);
int rfl4_switch_lim_get_rfFreq(int fd, short *buf);
int rfl4_switch_lim_get_rfFreqMax(int fd, short *buf);
int rfl4_switch_lim_set_rfFreqMax(int fd, short buf);
int rfl4_switch_lim_get_rfFreqMin(int fd, short *buf);
int rfl4_switch_lim_set_rfFreqMin(int fd, short buf);
int rfl4_switch_lim_get_fcAdjH(int fd, short *buf);
int rfl4_switch_lim_set_fcAdjH(int fd, short buf);
int rfl4_switch_lim_get_adcTestData(int fd, short *buf);
int rfl4_switch_lim_set_adcTestData(int fd, short buf);
int rfl4_switch_lim_get_sysControl(int fd, short *buf);
int rfl4_switch_lim_set_sysControl(int fd, short buf);
int rfl4_switch_lim_get_hardwareVersion(int fd, short *buf);
int rfl4_switch_lim_get_designerID(int fd, short *buf);
int rfl4_switch_lim_get_boardTemp(int fd, short *buf);
int rfl4_switch_lim_get_chipTemp(int fd, short *buf);
int rfl4_switch_lim_get_testControl(int fd, short *buf);
int rfl4_switch_lim_set_testControl(int fd, short buf);
int rfl4_switch_lim_get_testStatus(int fd, short *buf);
int rfl4_switch_lim_get_firmwareVersion(int fd, long *buf);
int rfl4_switch_lim_get_serialNumber(int fd, long *buf);
int rfl4_switch_lim_get_serialNumber_window(int fd, long buf[], int from, int to);
int rfl4_switch_lim_get_memMapVersion(int fd, long *buf);
int rfl4_switch_lim_get_diagInGain(int fd, short *buf);
int rfl4_switch_lim_set_diagInGain(int fd, short buf);
int rfl4_switch_lim_get_diagInOffset(int fd, short *buf);
int rfl4_switch_lim_set_diagInOffset(int fd, short buf);
int rfl4_switch_lim_get_diagOutGain(int fd, short *buf);
int rfl4_switch_lim_set_diagOutGain(int fd, short buf);
int rfl4_switch_lim_get_diagOutOffset(int fd, short *buf);
int rfl4_switch_lim_set_diagOutOffset(int fd, short buf);
int rfl4_switch_lim_get_dac1Gain(int fd, short *buf);
int rfl4_switch_lim_set_dac1Gain(int fd, short buf);
int rfl4_switch_lim_get_dac1Offset(int fd, short *buf);
int rfl4_switch_lim_set_dac1Offset(int fd, short buf);
int rfl4_switch_lim_get_dac2Gain(int fd, short *buf);
int rfl4_switch_lim_set_dac2Gain(int fd, short buf);
int rfl4_switch_lim_get_dac2Offset(int fd, short *buf);
int rfl4_switch_lim_set_dac2Offset(int fd, short buf);
int rfl4_switch_lim_get_dac3Gain(int fd, short *buf);
int rfl4_switch_lim_set_dac3Gain(int fd, short buf);
int rfl4_switch_lim_get_dac3Offset(int fd, short *buf);
int rfl4_switch_lim_set_dac3Offset(int fd, short buf);
int rfl4_switch_lim_get_dac1Gain2(int fd, short *buf);
int rfl4_switch_lim_set_dac1Gain2(int fd, short buf);
int rfl4_switch_lim_get_dac1Offset2(int fd, short *buf);
int rfl4_switch_lim_set_dac1Offset2(int fd, short buf);
int rfl4_switch_lim_get_powerDetVTAdj(int fd, short *buf);
int rfl4_switch_lim_set_powerDetVTAdj(int fd, short buf);
int rfl4_switch_lim_get_testMem(int fd, short *buf);
int rfl4_switch_lim_get_testMem_window(int fd, short buf[], int from, int to);
int rfl4_switch_lim_set_testMem(int fd, short buf);
int rfl4_switch_lim_set_testMem_window(int fd, short buf[], int from, int to);
int rfl4_switch_lim_get_VECTOR(int fd, short *buf);
int rfl4_switch_lim_set_VECTOR(int fd, short buf);
int rfl4_switch_lim_get_LEVEL(int fd, short *buf);
int rfl4_switch_lim_set_LEVEL(int fd, short buf);
int rfl4_switch_lim_get_memViewport(int fd, short *buf);
int rfl4_switch_lim_get_memViewport_window(int fd, short buf[], int from, int to);





int rfl4_switch_lim_get_control1(int fd, short *buf)
int rfl4_switch_lim_set_control1(int fd, short buf)
int rfl4_switch_lim_get_vmeIRQStatID(int fd, short *buf)
int rfl4_switch_lim_set_vmeIRQStatID(int fd, short buf)
int rfl4_switch_lim_get_vmeIRQLevel(int fd, short *buf)
int rfl4_switch_lim_set_vmeIRQLevel(int fd, short buf)
int rfl4_switch_lim_get_status1(int fd, short *buf)
int rfl4_switch_lim_get_ident(int fd, short *buf)
int rfl4_switch_lim_get_faults1(int fd, short *buf)
int rfl4_switch_lim_get_overflow1(int fd, short *buf)
int rfl4_switch_lim_get_pmAddr(int fd, long *buf)
int rfl4_switch_lim_get_obsAddr(int fd, long *buf)
int rfl4_switch_lim_get_memStatus(int fd, short *buf)
int rfl4_switch_lim_get_memControl1(int fd, short *buf)
int rfl4_switch_lim_set_memControl1(int fd, short buf)
int rfl4_switch_lim_get_memControl2(int fd, short *buf)
int rfl4_switch_lim_set_memControl2(int fd, short buf)
int rfl4_switch_lim_get_memControl3(int fd, short *buf)
int rfl4_switch_lim_set_memControl3(int fd, short buf)
int rfl4_switch_lim_get_nvMemStatus(int fd, short *buf)
int rfl4_switch_lim_get_nvMemControl(int fd, short *buf)
int rfl4_switch_lim_set_nvMemControl(int fd, short buf)
int rfl4_switch_lim_get_nvMemGetKey(int fd, short *buf)
int rfl4_switch_lim_set_nvMemSetKey(int fd, short buf)
int rfl4_switch_lim_get_nvMemCRCVal(int fd, short *buf)
int rfl4_switch_lim_get_rfLimitingLevel(int fd, short *buf)
int rfl4_switch_lim_set_rfLimitingLevel(int fd, short buf)
int rfl4_switch_lim_get_rfGainLevel(int fd, short *buf)
int rfl4_switch_lim_set_rfGainLevel(int fd, short buf)
int rfl4_switch_lim_get_fbSetPoint(int fd, short *buf)
int rfl4_switch_lim_set_fbSetPoint(int fd, short buf)
int rfl4_switch_lim_get_rfLimitingDelay(int fd, short *buf)
int rfl4_switch_lim_set_rfLimitingDelay(int fd, short buf)
int rfl4_switch_lim_get_loPwrMax(int fd, short *buf)
int rfl4_switch_lim_set_loPwrMax(int fd, short buf)
int rfl4_switch_lim_get_loPwrMin(int fd, short *buf)
int rfl4_switch_lim_set_loPwrMin(int fd, short buf)
int rfl4_switch_lim_get_inputsControl(int fd, short *buf)
int rfl4_switch_lim_set_inputsControl(int fd, short buf)
int rfl4_switch_lim_get_irqControl(int fd, short *buf)
int rfl4_switch_lim_set_irqControl(int fd, short buf)
int rfl4_switch_lim_get_vetoStatus(int fd, short *buf)
int rfl4_switch_lim_get_vetoControl(int fd, short *buf)
int rfl4_switch_lim_set_vetoControl(int fd, short buf)
int rfl4_switch_lim_get_status2(int fd, short *buf)
int rfl4_switch_lim_get_clockStatus(int fd, short *buf)
int rfl4_switch_lim_get_diagInPwr(int fd, short *buf)
int rfl4_switch_lim_get_diagOutPwr(int fd, short *buf)
int rfl4_switch_lim_get_diagInPhase(int fd, short *buf)
int rfl4_switch_lim_get_diagOutPhase(int fd, short *buf)
int rfl4_switch_lim_get_frevAdjDiagIn(int fd, short *buf)
int rfl4_switch_lim_set_frevAdjDiagIn(int fd, short buf)
int rfl4_switch_lim_get_frevAdjDiagOut(int fd, short *buf)
int rfl4_switch_lim_set_frevAdjDiagOut(int fd, short buf)
int rfl4_switch_lim_get_frevAdjH(int fd, short *buf)
int rfl4_switch_lim_set_frevAdjH(int fd, short buf)
int rfl4_switch_lim_get_adcTestControl(int fd, short *buf)
int rfl4_switch_lim_set_adcTestControl(int fd, short buf)
int rfl4_switch_lim_get_adcTestStatus(int fd, short *buf)
int rfl4_switch_lim_get_dac1Cal(int fd, short *buf)
int rfl4_switch_lim_get_dac2Cal(int fd, short *buf)
int rfl4_switch_lim_get_dac3Cal(int fd, short *buf)
int rfl4_switch_lim_get_adclkFreqMax(int fd, short *buf)
int rfl4_switch_lim_set_adclkFreqMax(int fd, short buf)
int rfl4_switch_lim_get_adclkFreqMin(int fd, short *buf)
int rfl4_switch_lim_set_adclkFreqMin(int fd, short buf)
int rfl4_switch_lim_get_frevDetector(int fd, short *buf)
int rfl4_switch_lim_set_frevDetector(int fd, short buf)
int rfl4_switch_lim_get_fcDetector(int fd, short *buf)
int rfl4_switch_lim_set_fcDetector(int fd, short buf)
int rfl4_switch_lim_get_loPwr(int fd, short *buf)
int rfl4_switch_lim_get_uP1V2(int fd, short *buf)
int rfl4_switch_lim_get_uP1V8(int fd, short *buf)
int rfl4_switch_lim_get_uP2V5(int fd, short *buf)
int rfl4_switch_lim_get_uP3V3(int fd, short *buf)
int rfl4_switch_lim_get_uP1V8A(int fd, short *buf)
int rfl4_switch_lim_get_uP5VA(int fd, short *buf)
int rfl4_switch_lim_get_uP8V(int fd, short *buf)
int rfl4_switch_lim_get_uM3V3(int fd, short *buf)
int rfl4_switch_lim_get_uM5VA(int fd, short *buf)
int rfl4_switch_lim_get_uP3V3D(int fd, short *buf)
int rfl4_switch_lim_get_uP5V(int fd, short *buf)
int rfl4_switch_lim_get_uP12V(int fd, short *buf)
int rfl4_switch_lim_get_uM12V(int fd, short *buf)
int rfl4_switch_lim_get_uP12VA(int fd, short *buf)
int rfl4_switch_lim_get_uM12VA(int fd, short *buf)
int rfl4_switch_lim_get_uP6VA(int fd, short *buf)
int rfl4_switch_lim_get_uM6VA(int fd, short *buf)
int rfl4_switch_lim_get_obsEvntCycleTime(int fd, long *buf)
int rfl4_switch_lim_get_pmEvntCycleTime(int fd, long *buf)
int rfl4_switch_lim_get_cycleTimeControl(int fd, short *buf)
int rfl4_switch_lim_set_cycleTimeControl(int fd, short buf)
int rfl4_switch_lim_get_timingSel1(int fd, short *buf)
int rfl4_switch_lim_set_timingSel1(int fd, short buf)
int rfl4_switch_lim_get_refPhase(int fd, short *buf)
int rfl4_switch_lim_get_pmObsFrzTrigger(int fd, short *buf)
int rfl4_switch_lim_set_pmObsFrzTrigger(int fd, short buf)
int rfl4_switch_lim_get_rfFreq(int fd, short *buf)
int rfl4_switch_lim_get_rfFreqMax(int fd, short *buf)
int rfl4_switch_lim_set_rfFreqMax(int fd, short buf)
int rfl4_switch_lim_get_rfFreqMin(int fd, short *buf)
int rfl4_switch_lim_set_rfFreqMin(int fd, short buf)
int rfl4_switch_lim_get_fcAdjH(int fd, short *buf)
int rfl4_switch_lim_set_fcAdjH(int fd, short buf)
int rfl4_switch_lim_get_adcTestData(int fd, short *buf)
int rfl4_switch_lim_set_adcTestData(int fd, short buf)
int rfl4_switch_lim_get_sysControl(int fd, short *buf)
int rfl4_switch_lim_set_sysControl(int fd, short buf)
int rfl4_switch_lim_get_hardwareVersion(int fd, short *buf)
int rfl4_switch_lim_get_designerID(int fd, short *buf)
int rfl4_switch_lim_get_boardTemp(int fd, short *buf)
int rfl4_switch_lim_get_chipTemp(int fd, short *buf)
int rfl4_switch_lim_get_testControl(int fd, short *buf)
int rfl4_switch_lim_set_testControl(int fd, short buf)
int rfl4_switch_lim_get_testStatus(int fd, short *buf)
int rfl4_switch_lim_get_firmwareVersion(int fd, long *buf)
int rfl4_switch_lim_get_serialNumber(int fd, long *buf)
int rfl4_switch_lim_get_serialNumber_window(int fd, long buf[], int from, int to)
int rfl4_switch_lim_get_memMapVersion(int fd, long *buf)
int rfl4_switch_lim_get_diagInGain(int fd, short *buf)
int rfl4_switch_lim_set_diagInGain(int fd, short buf)
int rfl4_switch_lim_get_diagInOffset(int fd, short *buf)
int rfl4_switch_lim_set_diagInOffset(int fd, short buf)
int rfl4_switch_lim_get_diagOutGain(int fd, short *buf)
int rfl4_switch_lim_set_diagOutGain(int fd, short buf)
int rfl4_switch_lim_get_diagOutOffset(int fd, short *buf)
int rfl4_switch_lim_set_diagOutOffset(int fd, short buf)
int rfl4_switch_lim_get_dac1Gain(int fd, short *buf)
int rfl4_switch_lim_set_dac1Gain(int fd, short buf)
int rfl4_switch_lim_get_dac1Offset(int fd, short *buf)
int rfl4_switch_lim_set_dac1Offset(int fd, short buf)
int rfl4_switch_lim_get_dac2Gain(int fd, short *buf)
int rfl4_switch_lim_set_dac2Gain(int fd, short buf)
int rfl4_switch_lim_get_dac2Offset(int fd, short *buf)
int rfl4_switch_lim_set_dac2Offset(int fd, short buf)
int rfl4_switch_lim_get_dac3Gain(int fd, short *buf)
int rfl4_switch_lim_set_dac3Gain(int fd, short buf)
int rfl4_switch_lim_get_dac3Offset(int fd, short *buf)
int rfl4_switch_lim_set_dac3Offset(int fd, short buf)
int rfl4_switch_lim_get_dac1Gain2(int fd, short *buf)
int rfl4_switch_lim_set_dac1Gain2(int fd, short buf)
int rfl4_switch_lim_get_dac1Offset2(int fd, short *buf)
int rfl4_switch_lim_set_dac1Offset2(int fd, short buf)
int rfl4_switch_lim_get_powerDetVTAdj(int fd, short *buf)
int rfl4_switch_lim_set_powerDetVTAdj(int fd, short buf)
int rfl4_switch_lim_get_testMem(int fd, short *buf)
int rfl4_switch_lim_get_testMem_window(int fd, short buf[], int from, int to)
int rfl4_switch_lim_set_testMem(int fd, short buf)
int rfl4_switch_lim_set_testMem_window(int fd, short buf[], int from, int to)
int rfl4_switch_lim_get_VECTOR(int fd, short *buf)
int rfl4_switch_lim_set_VECTOR(int fd, short buf)
int rfl4_switch_lim_get_LEVEL(int fd, short *buf)
int rfl4_switch_lim_set_LEVEL(int fd, short buf)
int rfl4_switch_lim_get_memViewport(int fd, short *buf)
int rfl4_switch_lim_get_memViewport_window(int fd, short buf[], int from, int to)

#ifdef __cplusplus
}
#endif

#endif /* _RFL4_SWITCH_LIM_REGS_H */
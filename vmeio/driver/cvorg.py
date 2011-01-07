#!   /usr/bin/env	python
#    coding: utf8

import pyvme
import sys

class CvorgRegs:
    int_sources		    = 0x000     # Bit pattern Interrupt sources Read only (cleared on read)
    int_enable_mask		= 0x004     # Bit pattern Interrupt enable mask Read/Write
    int_level		    = 0x008     # Unsigned int Interrupt level Read/Write
    int_vector	    	= 0x00C     # Unsigned int Interrupt vector Read/Write
    vhdl_version		= 0x010     # BCD VHDL version Read only
    pcb_serial_msb		= 0x014     # Unsigned int PCB serial number (MSBs) Read only
    pcb_serial_lsb		= 0x018     # Unsigned int PCB serial number (LSBs) Read only
    temperature		    = 0x01C     # Signed Temperature Read only
    adc_value	    	= 0x020     # Bit pattern ADC value Read only
    software_triggers	= 0x024     # Bit pattern Software triggers Write only
    sampling_clk_freq	= 0x028     # Unsigned int Sampling clock frequency Read only
    clk_gen_control		= 0x02C     # Bit pattern Clock generator control Read/Write
    channel_status		= 0x040     # Bit pattern Channel Status Read only
    channel_config		= 0x044     # Bit pattern Channel config Read/Write
    dac_value		    = 0x048     # Unsigned int DAC value Read/Write
    sram_start_addr		= 0x04C     # Unsigned int SRAM start address Read/Write
    sram_data	    	= 0x050     # Unsigned int SRAM data Read/Write
    dac_control 		= 0x054     # Bit pattern DAC control Read/Write
    num_seq_cycles		= 0x058     # Unsigned int Number of sequence cycles Read/Write
    dac_correction		= 0x05C     # Bit pattern DAC correction (Offset/Gain) Read/Write

    wf_start_addr =     [ 0x60 + 0x10 * i for i in range(26) ]		# Waveform start address Read/Write
    wf_length     =     [ 0x64 + 0x10 * i for i in range(26) ]      # Waveform length Read/Write
    wf_nr_cycles  =     [ 0x68 + 0x10 * i for i in range(26) ]      # Waveform nb. of cycles Read/Write
    wf_next_wf_nb_gain = [ 0x60 + 0x6c * i for i in range(26) ]     # Next waveform nb + gain Read/Write

if __name__ == '__main__':

    # first do a proper insmod as per cvorg_install.sh

    cvorg = pyvme.Vme(lun=0)
    Regs = CvorgRegs    # just a short alias
    tstvalue = int(sys.argv[1], 16)

    # write and read number of waveform cycles of 3rd wf
    ret, contents = cvorg.raw_read(Regs.wf_nr_cycles[3])
    print 'register contained 0x%08x' % contents

    cvorg.raw_write(Regs.wf_nr_cycles[3], tstvalue)
    ret, contents = cvorg.raw_read(Regs.wf_nr_cycles[3])
    print 'register contains now 0x%08x' % contents

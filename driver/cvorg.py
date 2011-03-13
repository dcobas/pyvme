#!   /usr/bin/env	python
#    coding: utf8

import sys
import pyvme

def vmod12e16_test(d, channel, ampli, offset=0x0):
    """perform a conversion from a vmod12e16 given channel and
    amplification factor (0, 1, 2, 3). Optionally, an offset
    within the MODULBUS address space can be specified
    """
    # print 'set device return code ', d.set_device(base_address=0x6200,
    #        address_modifier=0x29, data_width=2, length=0x200)

    def conversion_command(channel, ampli):
        return (ampli & 0x3 << 4) | channel & 0xf

    def data_to_float(data):
        return (data & ((1<<12)-1)) / (2.0**12) * 20 - 10

    def is_ready(ready_reg):
	return ready_reg & (1<<15) == 0

    control            =  offset + 0x0
    adc_data           =  offset + 0x0
    adc_ready          =  offset + 0x04
    interrupt_disable  =  offset + 0x06

    # enable interrupt mode
    print 'enable interrupt ', 
    print d.raw_write(interrupt_disable, 0, bytes=2)

    # prepare conversion
    command = conversion_command(channel, ampli)
    print d.raw_write(control, command, bytes=2)

    # wait interrupt
    print d.irq_wait(timeout=1000)

    # ready?
    ret, ready = d.raw_read(adc_ready, bytes=2)
    print 'adc ready = 0x%04x,%s ready' % ( ready,
        not is_ready(ready) and " not" or "")

    # converted value
    ret, data = d.raw_read(adc_data, bytes=2)
    print 'value = %12.5f' % data_to_float(data)

def vmod12a2_test(fd, channel, voltage, offset=0x0000):

    def volt_to_12bit(voltage):
        int((voltage + 10.0) / 20.0) & 0xfff

    register = channel << 1
    fd.raw_write(register, volt_to_12bit(voltage))

def my_vmod12e16_test_program():
    d = pyvme.Vme(0)
    if len(sys.argv) < 2:
        print 'usage: vmod12e16_test channel[0..7] ampli_factor[0..3]'
        return
    channel = int(sys.argv[1])
    ampli = int(sys.argv[2])
    vmod12e16_test(d, channel, ampli, offset=0x200)

def my_vmod12a2_test_program():
    d = pyvme.Vme(0)
    if len(sys.argv) < 2:
        print 'usage: vmod12a2_test channel[0..1] voltage[-10.0..10.0]'
        return
    channel = int(sys.argv[1])
    voltage = float(sys.argv[2])
    vmod12a2_test(d, channel, voltage, offset=0x0000)

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

def my_cvorg_test_program():

    if len(sys.argv) < 2:
        print 'usage: %s hex_value' % sys.argv[0]
        return

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


if __name__ == '__main__':
    import os.path
   
    progname = os.path.basename(sys.argv[0])
    if progname == 'vmod12a2.py':
        my_vmod12a2_test_program()
    elif progname == 'vmod12e16.py':
        my_vmod12e16_test_program()
    elif progname == 'cvorg.py':
        my_cvorg_test_program()
    else:
        print 'usage: python [vmod12a2.py|vmod12e16.py|cvorg.py] args...'


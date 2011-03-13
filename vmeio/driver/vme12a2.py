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

def my_cvorg_test_program():
    d = pyvme.Vme(0)
    print d.raw_read(0x6c)
    print d.raw_write(0x6c, 0x01234567)
    print d.raw_read(0x6c)

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


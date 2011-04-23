#!   /usr/bin/env	python
#    coding: utf8

from ctypes import *
import os
import cmd
import readline
import struct

class vmeio_read_buf_s(Structure):
    _fields_ = [
    ('logical_unit', c_int),
    ('interrupt_mask', c_int),
    ('interrupt_count', c_int),
    ]

class vmeio_get_mapping_s(Structure):
    _fields_ = [
   ('lun', c_int),	     	    # Logical unit number
   ('level', c_int),	     	# Interrupt level
   ('vector', c_int),	     	# Interrupt vector
   ('base_address1', c_int),   	# First VME base address
   ('base_address2', c_int),  	# Second VME base address or zero

   ('am1', c_int),	         	# First address modifier
   ('am2', c_int),	    	    # Second address modifier or zero
   ('data_width1', c_int),    	# First data width
   ('data_width2', c_int),		# Second data width or zero
   ('size1', c_int),	      	# First map size
   ('size2', c_int),	   	    # Second map size or zero
   ('isrc', c_int),	         	# Offset of isrc in vme1 to be read in the isr
   ]

class vme_mapping(Structure):
    _fields_ = [
	('window_num', c_int),
	('kernel_va', c_void_p),
	('user_va', c_void_p),
	('fd', c_int),
	('window_enabled', c_int),
	('data_width', c_int),
	('am', c_int),
	('read_prefetch_enabled', c_int),
	('read_prefetch_size', c_int),
	('v2esst_mode', c_int),
	('bcast_select', c_int),
	('pci_addru', c_int),
	('pci_addrl', c_int),
	('sizeu', c_int),
	('sizel', c_int),
	('vme_addru', c_int),
	('vme_addrl', c_int),
    ]

class vmeio_get_mapping(Structure):
    _fields_ = [
    ('mapnum', c_int),
    ('map', vme_mapping),
    ]

vmeioMAX_BUF = 8192

class vmeio_riob_s(Structure):
    _fields_ = [
    ('mapnum', c_int),	   	# Mapping number 1..2
    ('offset', c_int),	   	# Byte offset in map
    ('wsize', c_int),	   	# The number of bytes to read
    ('buffer', c_void_p),	# Pointer to data area
    ]

class vmeio_dma_op(Structure):
    _fields_ = [
    ('am', c_int),				# address modifier, defines transfer mode
    ('data_width', c_int),		# transfer data width
    ('address', c_int),			# vme address of start transfer
    ('byte_length', c_int),		# transfer length in bytes
    ('buffer', c_void_p),		# pointer to transfer to/from
    ('direction', c_int),		# from/to device
    ]

# define ioctl numbers here

VMEIO_GET_DEBUG     = 0x80045602
VMEIO_SET_DEBUG     = 0x40045601
VMEIO_GET_VERSION   = 0x80045603
VMEIO_GET_TIMEOUT   = 0x80045605
VMEIO_SET_TIMEOUT   = 0x40045604
VMEIO_RAW_READ      = 0xc0105607
VMEIO_RAW_WRITE     = 0xc0105608
VMEIO_RAW_READ_DMA  = 0xc0105609
VMEIO_RAW_WRITE_DMA = 0xc010560a
VMEIO_READ_DMA      = 0x4018560c
VMEIO_WRITE_DMA     = 0x4018560d
VMEIO_GET_MAPPING   = 0xc044560e

libc = CDLL('/lib/libc.so.6')


data_width_format = {
    32 : 'I',
    16 : 'H',
     8 : 'B',
}

def mapnum2dw(fd, mapno):
    s = vmeio_get_mapping(mapnum=mapno, map=vme_mapping())
    if 0 > libc.ioctl(fd, VMEIO_GET_MAPPING, byref(s)):
        print 'Failed to get data width for address space ', mapno
        return -1
    return s.map.data_width

def raw_read(fd, mapnum, offset, items):
    data_width =  mapnum2dw(fd, mapnum)
    bpw = data_width / 8
    print 'bytes per word = ', bpw
    buf = create_string_buffer(bpw*items+1)
    format = items * data_width_format[data_width]
    print 'format = ', format
    s = vmeio_riob_s(mapnum=mapnum, offset=offset, wsize=items, buffer=addressof(buf))
    print libc.ioctl(fd, VMEIO_RAW_READ, byref(s))
    ret = struct.unpack(format, buf[:-1])
    print ret
    return ret

device_name = '/dev/vmeio.%d'

class TestProgram(cmd.Cmd):

    def __init__(self):
        cmd.Cmd.__init__(self)
        self.do_open(0)

    def do_open(self, arg):
        try:
            self.lun = int(arg)
        except:
            print 'please provide a valid LUN'
            return
        self.devname = device_name % self.lun
        self.fd = libc.open(self.devname, os.O_RDWR)
        if not self.fd < 0:
            print 'open lun %d with fd %d' % (self.lun, self.fd)
            self.do_EOF('')
        else:
            print 'could not open %s' % self.devname
            self.do_EOF('')


    def do_raw_read(self, arg):
        """raw_read mapnum offset nitems

        Read nitems word at offset of mapping mapnum
        """
        args = arg.split()
        if len(args) != 3:
            print 'usage: raw_read mapnum offset items'
            return
        try:
            mapnum, offset, items = map(int, args)
        except:
            print 'mapnum, offset and items must be integers'
            return
        if not self.fd:
            print 'open a lun first'
            return

        byte_width = mapnum2dw(self.fd, mapnum)/8
        val = raw_read(self.fd, mapnum, offset, items)
        for i in range(items):
            fmt = '%%08x: 0x%%0%dx' % (byte_width*2)
            print fmt % (offset, val[i])
            offset += byte_width

    def do_params(self, arg):
        """params mapnum        show device parameters
        """
        
        if not arg:
            self.do_params(1)
            self.do_params(2)
            return

        try:
            mapnum = int(arg)
        except:
            print 'please provide a valid mapnum'
            return 

        s = vmeio_get_mapping(mapnum=mapnum, map=vme_mapping())
        if 0 > libc.ioctl(self.fd, VMEIO_GET_MAPPING, byref(s)):
            print 'VMEIO_GET_MAPPING failed'
            return
        print 'mapping = %d, data_width = %d, size = 0x%x, am = 0x%x' % (
            mapnum,
            s.map.data_width,
            s.map.sizel,
            s.map.am, )

    def do_EOF(self, arg):
        print
        return 1

if __name__ == '__main__':
    tp = TestProgram()
    tp.prompt = 'vmeio> '
    tp.cmdloop()


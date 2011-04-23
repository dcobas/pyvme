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

vmeioMAX_BUF = 8192

class vmeio_riob_s(Structure):
    _fields_ = [
    ('mapnum', c_int),	   	# Mapping number 1..2
    ('offset', c_int),	   	# Byte offset in map
    ('bsize', c_int),	   	# The number of bytes to read
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

def raw_read(fd, mapnum, offset, bsize):
    buf = create_string_buffer(bsize+1)
    s = vmeio_riob_s(mapnum=mapnum, offset=offset, bsize=bsize, buffer=addressof(buf))
    libc.ioctl(fd, VMEIO_RAW_READ, byref(s))
    return struct.unpack('I', buf[:-1])[0]

device_name = '/dev/vmeio.%d'

class TestProgram(cmd.Cmd):

    def __init__(self):
        cmd.Cmd.__init__(self)
        self.fd = None

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
        else:
            print 'could not open %s' % self.devname

    def do_hello(self, arg):
        print 'Hello'
        print arg

    def help_hello(self):
        print "Prints hello"

    def do_raw_read(self, arg):
        """raw_read mapnum offset bsize

        Read bsize bytes at offset of mapping mapnum
        """
        args = arg.split()
        if len(args) != 3:
            print 'usage: raw_read mapnum offset bsize'
            return
        try:
            mapnum, offset, bsize = map(int, args)
        except:
            print 'mapnum, offset and bsize must be integers'
        if not self.fd:
            print 'open a lun first'
            return
        val = raw_read(self.fd, mapnum, offset, bsize)
        print '0x%08x' % val

    def do_EOF(self, arg):
        print
        return 1

if __name__ == '__main__':
    tp = TestProgram()
    tp.prompt = 'vmeio> '
    tp.cmdloop()



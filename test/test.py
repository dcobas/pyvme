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

class vmeio_riob(Structure):
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

class encore_reginfo(Structure):
    _fields_ = [
	('name',			c_char*17),
	('rwmode',			c_char*5),
	('block',			c_int),
	('block_address_space',			c_int),
	('block_offset',	c_int),
	('register_offset',	c_int),
	('offset',			c_int),
	('wordsize',		c_char*7),
	('depth',			c_int),
	('description',		c_char*81),
	('data_width',		c_int),
    ]

# define ioctl numbers here

VMEIO_GET_DEBUG 	= 0x80045602
VMEIO_SET_DEBUG 	= 0x40045601
VMEIO_GET_VERSION   = 0x80045603
VMEIO_GET_TIMEOUT   = 0x80045605
VMEIO_SET_TIMEOUT   = 0x40045604
VMEIO_RAW_READ  	= 0xc0145607
VMEIO_RAW_WRITE 	= 0xc0145608
VMEIO_RAW_READ_DMA  = 0xc0145609
VMEIO_RAW_WRITE_DMA = 0xc014560a
VMEIO_READ_DMA  	= 0x4018560c
VMEIO_WRITE_DMA 	= 0x4018560d
VMEIO_GET_MAPPING  	= 0xc044560e
VMEIO_GET_NREGS 	= 0x8004560e
VMEIO_GET_REGINFO  	= 0x40045610

libc = CDLL('/lib/libc.so.6')

data_width_format = {
    32 : 'I',
    16 : 'H',
     8 : 'B',
}

device_name = '/dev/vmeio.%d'

class TestProgram(cmd.Cmd):

    def __init__(self):
        cmd.Cmd.__init__(self)
        self.fd = None
        self.do_lun(0)

    def do_lun(self, arg, device_name=device_name):
        """lun [lun number]     open a LUN"""

        if arg == '':
            print 'open: lun = %d, fd = %d' % (self.lun, self.fd)
            return
        try:
            lun = int(arg)
        except:
            print 'lun must be an integer'
            return

        devname = device_name % lun
        fd = libc.open(devname, os.O_RDWR)
        if fd < 0:
            print 'could not open %s' % self.devname
            if not self.fd:
                return 1
            else:
                return
        self.fd = fd
        self.lun = lun
        self.devname = devname
        print 'open: lun %d, fd %d' % (self.lun, self.fd)

        self.data_width = [ 0, 0 ]
        for mapnum in [ 1, 2 ]:
            gm = vmeio_get_mapping(mapnum=mapnum, map=vme_mapping())
            libc.ioctl(self.fd, VMEIO_GET_MAPPING, byref(gm))
            self.data_width[mapnum-1] = gm.map.data_width

    def do_raw_read(self, arg):
        """raw_read mapnum offset nitems

        Read nitems words at offset of mapping mapnum
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
        if mapnum not in [ 1, 2 ]:
            print 'mapnum must be 1 or 2'
            return

        data_width =  self.data_width[mapnum-1]
        byte_width = data_width / 8
        buf = create_string_buffer(byte_width*items+1)
        s = vmeio_riob(mapnum=mapnum, 
            offset=offset, wsize=items, buffer=addressof(buf), data_width=0)
        if libc.ioctl(self.fd, VMEIO_RAW_READ, byref(s)) < 0:
            print 'VMEIO_RAW_READ failed!'
            return
        format = items * data_width_format[data_width]
        val = struct.unpack(format, buf[:-1])

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



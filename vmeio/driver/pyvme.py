#!   /usr/bin/env	python
#    coding: utf8

import sys
from ctypes import *
from struct import *

library_path = './libvmeio.so'
lib = CDLL(library_path)

class Riob(Structure):
    _fields_ = [
       ('winum',  c_int),    # Window number 1..2
       ('offset', c_int),    # Byte offset in map
       ('bsize',  c_int),    # The number of bytes to read
       ('buffer', c_void_p), # Pointer to data area
    ]
    VMEIO_GET_DEBUG = 0x80045602
    VMEIO_SET_DEBUG = 0x40045601
    VMEIO_GET_VERSION = 0x80045603
    VMEIO_GET_TIMEOUT = 0x80045605
    VMEIO_SET_TIMEOUT = 0x40045604
    VMEIO_GET_DEVICE = 0x80345606
    VMEIO_RAW_READ = 0xc0205607
    VMEIO_RAW_WRITE = 0xc0205608
    VMEIO_RAW_READ_DMA = 0xc0205609
    VMEIO_RAW_WRITE_DMA = 0xc020560a
    VMEIO_SET_DEVICE = 0x40345606


class GetWindow(Structure):
    _fields_ = [
    ('lun',  c_int),    # Logical unit number
    ('lvl',  c_int),    # Interrupt level
    ('vec',  c_int),    # Interrupt vector
    ('vme1', c_int),    # First VME base address
    ('vme2', c_int),    # Second VME base address or zero
    ('amd1', c_int),    # First address modifier
    ('amd2', c_int),    # Second address modifier or zero
    ('dwd1', c_int),    # First data width
    ('dwd2', c_int),    # Second data width or zero
    ('win1', c_int),    # First window size
    ('win2', c_int),    # Second window size or zero
    ('nmap', c_int),    # No map window flag, 1=DMA only
    ('isrc', c_int),    # Offset of isrc in vme1 to be read in the isr
    ]

class ReadBuf(Structure):
    _fields_ = [
    ( 'logical_unit', c_int ),      # Logical unit number for interrupt
    ( 'interrupt_mask', c_int ),    # Interrupt enable/source mask
    ( 'interrupt_count', c_int ),   # Current interrupt counter value
    ]


def int_to_buffer(value, bytes):
    fmt = ' BH L'[bytes]
    return pack(fmt, value)

def buffer_to_int(buffer, bytes):
    fmt = ' BH L'[bytes]
    return unpack(fmt, buffer[:bytes])[0]

class Vme(object):
    """Represent a vme device bound to the vmeio driver"""

    def __init__(self, lun):
        """Create a Vme device object by lun number, as configured
        in the vmeio driver insmod line
        """
        self.open(lun)

    def open(self, lun=0):
        """Open a Vme device object by lun number, as configured
        in the vmeio driver insmod line
        """
        self.lun = lun
        self.fd = lib.vmeio_open(lun)
        return self.fd

    def close(self):
        """Close the Vme device object"""
        return lib.vmeio_close(self.fd)

    def raw_read(self, offset, bytes=4, address_space=1):
        """Perform a vme read operation at offset 'offset'

        Data width can be specified optionally by the optional bytes
        parameter (default 4, i.e. 32 bits) and also the address space
        can be specified (default first one)

        The return value is a pair (ret, value), where 'value' is the
        integer value read, and ret is < 0 in case of error, 0 otherwise
        """
        buffer = create_string_buffer(bytes)
        riob = Riob(address_space, offset, bytes, cast(buffer, c_void_p))
        ret = lib.vmeio_raw_read(self.fd, byref(riob))
        return ret, buffer_to_int(buffer, bytes)

    def raw_write(self, offset, value, bytes=4, address_space=1):
        """Perform a vme write operation of datum 'value' at register 'offset'

        Data width can be specified optionally by the optional bytes
        parameter (default 4, i.e. 32 bits) and also the address space
        can be specified (default first one).

        Returns < 0 on error, 0 on success
        """
        buffer = int_to_buffer(value, bytes)
        riob = Riob(address_space, offset, bytes, cast(buffer, c_void_p))
        ret = lib.vmeio_raw_write(self.fd, byref(riob))
        return ret

    def set_device(self, address_modifier, base_address, data_width, length):
        """Change default mapping at address space 1

        Not implemented yet
        """
        win = GetWindow(vme1=base_address, amd1=address_modifier, 
                        dwd1=data_width, win1=length)
        return lib.vmeio_set_device(self.fd, byref(win))

    def irq_wait(self, timeout=0):
        """Wait for an interrupt 

        Vector and level are specified at vmeio install time by the
        vecs and lvls parameters. This call blocks until an interrupt
        occurs. If timeout is given, the function exits when the timeout
        expires if no interrupt occurs; the default behaviour with
        timeout 0 is to wait forever
        """
        return lib.vmeio_wait_irq(self.fd, timeout)


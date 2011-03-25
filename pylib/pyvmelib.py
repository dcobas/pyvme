#!  /user/dcobas/2.6.6/bin/python

import struct
from ctypes import *

class vme_mapping(Structure):
    _fields_ = [
	("window_num",		c_int),

	# Reserved for kernel use
	("*kernel_va",		c_void_p),

	# Reserved for userspace
	("*user_va",		c_void_p),
	("fd",		        c_int),

	# Window settings
	("window_enabled",		c_int),
	("data_width",		    c_int),
	("am",	            	c_int),
	("read_prefetch_enabled",	c_int),
	("read_prefetch_size",	c_int),
	("v2esst_mode",		    c_int),
	("bcast_select",		c_int),
	("pci_addru",	    	c_uint),
	("pci_addrl",		    c_uint),
	("sizeu",	    	    c_uint),
	("sizel",	        	c_uint),
	("vme_addru",		    c_uint),
	("vme_addrl",	    	c_uint),
    ]

class vme_dma_ctrl(Structure):
    _fields_ = [
	("vme_block_size",		c_int),
	("vme_backoff_time",	c_int),
	("pci_block_size",		c_int),
	("pci_backoff_time",	c_int),
    ]

class vme_dma_attr(Structure):
    _fields_ = [
	("data_width",		c_int),
	("am",	        	c_int),
	("v2esst_mode",		c_int),
	("bcast_select",	c_uint),
	("addru",		    c_uint),
	("addrl",	    	c_uint),
    ]

class vme_dma(Structure):
    _fields_ = [
	("status",		c_uint),
	("length",		c_uint),
	("novmeinc",	c_uint),
	("dir",		    c_uint),

	("src",		vme_dma_attr),
	("dst",		vme_dma_attr),

	("ctrl",		vme_dma_ctrl),
    ]

libvmebus = CDLL("./libvmebus.so")
vme_map   = libvmebus.vme_map
vme_map.restype = c_void_p
vme_unmap = libvmebus.vme_unmap
vme_dma_read  = libvmebus.vme_dma_read
vme_dma_write = libvmebus.vme_dma_write


# vme_read_prefetch_size's
(
    VME_PREFETCH_2,
	VME_PREFETCH_4,
	VME_PREFETCH_8,
	VME_PREFETCH_16 ) = range(4)

# vme_data_width's
VME_D8	= 8
VME_D16	= 16
VME_D32	= 32
VME_D64	= 64

# vme dma direction
VME_DMA_TO_DEVICE = 1
VME_DMA_FROM_DEVICE = 2

# vme_2esst_mode's
VME_SST160, VME_SST267, VME_SST320  = range(3)

# vme_address_modifier's
VME_A64_MBLT           =  0x00
VME_A64_SCT            =  0x01
VME_A64_BLT            =  0x03
VME_A64_LCK            =  0x04
VME_A32_LCK            =  0x05
VME_A32_USER_MBLT      =  0x08
VME_A32_USER_DATA_SCT  =  0x09
VME_A32_USER_PRG_SCT   =  0x0a
VME_A32_USER_BLT       =  0x0b
VME_A32_SUP_MBLT       =  0x0c
VME_A32_SUP_DATA_SCT   =  0x0d
VME_A32_SUP_PRG_SCT    =  0x0e
VME_A32_SUP_BLT        =  0x0f
VME_2e6U               =  0x20
VME_2e3U               =  0x21
VME_A16_USER           =  0x29
VME_A16_LCK            =  0x2c
VME_A16_SUP            =  0x2d
VME_CR_CSR             =  0x2f
VME_A40_SCT            =  0x34
VME_A40_LCK            =  0x35
VME_A40_BLT            =  0x37
VME_A24_USER_MBLT      =  0x38
VME_A24_USER_DATA_SCT  =  0x39
VME_A24_USER_PRG_SCT   =  0x3a
VME_A24_USER_BLT       =  0x3b
VME_A24_SUP_MBLT       =  0x3c
VME_A24_SUP_DATA_SCT   =  0x3d
VME_A24_SUP_PRG_SCT    =  0x3e
VME_A24_SUP_BLT        =  0x3f


# vme_block_size's
(
    VME_DMA_BSIZE_32,
    VME_DMA_BSIZE_64,
    VME_DMA_BSIZE_128,
    VME_DMA_BSIZE_256,
    VME_DMA_BSIZE_512,
    VME_DMA_BSIZE_1024,
    VME_DMA_BSIZE_2048,
    VME_DMA_BSIZE_4096 ) = range(8)

# vme_dma_backoff's
(
    VME_DMA_BACKOFF_0,
    VME_DMA_BACKOFF_1,
    VME_DMA_BACKOFF_2,
    VME_DMA_BACKOFF_4,
    VME_DMA_BACKOFF_8,
    VME_DMA_BACKOFF_16,
    VME_DMA_BACKOFF_32,
    VME_DMA_BACKOFF_64 ) = range(8)

def swap32be(value):
    tmp = struct.pack('>L', value)
    return struct.unpack('<L', tmp)[0]

def swap16be(value):
    tmp = struct.pack('>H', value)
    return struct.unpack('<H', tmp)[0]

if __name__ == '__main__':

    lib = CDLL('./libvmebus.so')

    def vme_map(am, data_width, base_address, size):
        
        mapping = vme_mapping()
        mapping.am                     =  am
        mapping.data_width             =  data_width
        mapping.vme_addru              =  0
        mapping.vme_addrl              =  base_address
        mapping.sizeu                  =  0
        mapping.sizel                  =  size
        mapping.read_prefetch_enabled  =  0
        mapping.bcast_select           =  0
        mapping.window_num             =  0

        return lib.vme_map(byref(mapping), 1)

    def vme_dma_read(vme_addr, am, ptr, size):
        desc = vme_dma();
        desc.dir = VME_DMA_FROM_DEVICE
        desc.length = size
        desc.dst.addru = 0
        desc.dst.addrl = ptr
        desc.src.addru = 0
        desc.src.addrl = vme_addr
        desc.src.am = am
        desc.src.v2esst_mode = VME_SST160
        desc.src.data_width = VME_D32
        desc.ctrl.pci_block_size = VME_DMA_BSIZE_2048
        desc.ctrl.pci_backoff_time = VME_DMA_BACKOFF_0
        desc.ctrl.vme_block_size = VME_DMA_BSIZE_2048
        desc.ctrl.vme_backoff_time = VME_DMA_BACKOFF_0

        return lib.vme_dma_read(byref(desc))

    def vme_dma_write(vme_addr, am, ptr, size):
        desc = vme_dma();
        desc.dir = VME_DMA_TO_DEVICE
        desc.length = size
        desc.src.addru = 0
        desc.src.addrl = ptr
        desc.dst.addru = 0
        desc.dst.addrl = addr
        desc.dst.am = am
        desc.dst.v2esst_mode = VME_SST160
        desc.dst.data_width = VME_D32
        desc.ctrl.pci_block_size = VME_DMA_BSIZE_2048
        desc.ctrl.pci_backoff_time = VME_DMA_BACKOFF_0
        desc.ctrl.vme_block_size = VME_DMA_BSIZE_2048
        desc.ctrl.vme_backoff_time = VME_DMA_BACKOFF_0

        return lib.vme_dma_write(byref(desc))

    addr = vme_map(am=0x9, data_width=VME_D32, base_address=0x10000000, size=0x10000)
    print 'Mapped at userspace vaddr 0x%08x' % addr
    p = cast(addr, POINTER(c_uint))
    for i in range(20):
        print 'register[%2d] = 0x%08x' % (i, swap32be(p[i]),)

    wval = c_ulong(0xBABEBABE)
    p_wval = id(wval)
    rval = c_ulong(42)
    p_rval = id(rval)
    addr = 0x10000000
    amod_r = 0x9
    amod_w = 0x9
    sz = 2
    ret = vme_dma_write(vme_addr=addr, am=amod_w, ptr=p_wval, size=sz)
    if (ret == -1):
        print 'Failed to do DMA read'
    else:
        print 'Written: 0x%08x' % wval.value
    ret = vme_dma_read(vme_addr=0x10000000, am=amod_r, ptr=p_rval, size=sz)
    print ret
    if (ret != 0):
        print 'Failed to do DMA write'
    else:
        print 'Read: 0x%08x' % rval.value

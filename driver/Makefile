CPU=L865
include /acc/dsc/src/co/Make.common

# KVER=2.6.24.7-rt21
# KVER=2.6.24.7-rt27
# KVER=2.6.29.4-rt15
# KVER=2.6.31.6-rt19

KVER=2.6.24.7-rt27

KERNELSRC=/acc/sys/$(CPU)/usr/src/kernels/$(KVER)
DRIVER_NAME = "vmeio"
COMPILE_TIME = $(shell date +%s)
CFLAGS += -I /acc/local/L865/include/
CFLAGS += -DCOMPILE_TIME=$(COMPILE_TIME) -g -Wall -fPIC

all: libs modules

libs: vmeio_support.so vmeio_support.a libencore.o

pyvme: libvmeio.so doc

libvmeio.so: libvmeio.o
	$(CC) -shared -o $@ $^
vmeio_support.so: vmeio_support.o
	-$(RM) $@
	$(CC) $(CFLAGS) -o $@ -shared $^
vmeio_support.a: vmeio_support.o
	-$(RM) $@
	$(AR) $(ARFLAGS) $@ $^
	$(RANLIB) $@
libencore.a: libencore.o
	-$(RM) $@
	$(AR) $(ARFLAGS) $@ $^
	$(RANLIB) $@

doc:
	pydoc -w ./pyvme.py
	pdflatex brka

modules: 
	cp Module.symvers.vmebus Module.symvers
	make -C $(KERNELSRC) M=`pwd` KVER=$(KVER) 'DRIVER_NAME=$(DRIVER_NAME)' modules
clean:
	rm -f *.o *.a *.pyc *.so \
		brka.aux brka.dvi brka.log
	rm -f pyvme.html
	make -C $(KERNELSRC) M=`pwd` clean


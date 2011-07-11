# generated automatically by encore, date 2011-07-11 10:31:44.393437

CPU=L865
KVER=2.6.24.7-rt27
KERNELSRC=/acc/sys/$(CPU)/usr/src/kernels/$(KVER)
include /acc/dsc/src/co/Make.auto

DRIVER = rfl4_switch_lim
DRIVER_NAME = "$(DRIVER)"
LIBNAME = lib_$(DRIVER)

COMPILE_TIME = $(shell date +%s)
CFLAGS= -g -Wall -I. -I ../../driver
CFLAGS += -I /acc/local/$(CPU)/include/
CFLAGS += -DCOMPILE_TIME=$(COMPILE_TIME) -g -Wall -fPIC

MODLIBS = $(LIBNAME).$(CPU).a $(LIBNAME).$(CPU).so
MODLIBOBJS = $(LIBNAME).$(CPU).o $(DRIVER)_regs.$(CPU).o vmeio_support.$(CPU).o

SRCS=$(LIBNAME).c $(LIBNAME).h

all: modules libs
libs: $(MODLIBS) $(VMELIBS)
modules:
	cp Module.symvers.vmebus Module.symvers
	make -C $(KERNELSRC) M=`pwd` KVER=$(KVER) 'DRIVER_NAME=$(DRIVER_NAME)' modules
clean:
	rm -f *.o *.a *.so
	make -C $(KERNELSRC) M=`pwd` clean

$(LIBNAME).$(CPU).o: $(SRCS)

$(LIBNAME).$(CPU).a: $(MODLIBOBJS)
	-$(RM) $@
	$(AR) $(ARFLAGS) $@ $^
	$(RANLIB) $@

$(LIBNAME).$(CPU).so: $(MODLIBOBJS)
	-$(RM) $@
	$(CC) $(CFLAGS) -o $@ -shared $^

lib$(DRIVER).$(CPU).so: lib$(DRIVER).$(CPU).o
	$(CC) -shared -o $@ $^


vmeio_support.$(CPU).o: vmeio_support.c
vmeio_support.$(CPU).so: vmeio_support.$(CPU).o
	-$(RM) $@
	$(CC) $(CFLAGS) -o $@ -shared $^

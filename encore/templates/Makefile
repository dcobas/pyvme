# generated automatically by encore, date %(date)s

CPU=L865
KVER=2.6.24.7-rt27
KERNELSRC=/acc/sys/$(CPU)/usr/src/kernels/$(KVER)
include /acc/dsc/src/co/Make.auto

DRIVER = %(driver_name)s
DRIVER_NAME = "$(DRIVER)"
LIBNAME = lib_$(DRIVER)

COMPILE_TIME = $(shell date +%%s)
CFLAGS= -g -Wall -I. -I ../../driver
CFLAGS += -I /acc/local/$(CPU)/include/
CFLAGS += -DCOMPILE_TIME=$(COMPILE_TIME) -g -Wall -fPIC

MODLIBS = $(LIBNAME).$(CPU).a $(LIBNAME).$(CPU).so
VMELIBS = vmeio_support.$(CPU).so vmeio_support.$(CPU).a libencore.$(CPU).o

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

$(LIBNAME).$(CPU).a: $(LIBNAME).$(CPU).o
	-$(RM) $@
	$(AR) $(ARFLAGS) $@ $^
	$(RANLIB) $@

$(LIBNAME).$(CPU).so: $(LIBNAME).$(CPU).o
	-$(RM) $@
	$(CC) $(CFLAGS) -o $@ -shared $^

lib$(DRIVER).$(CPU).so: lib$(DRIVER).$(CPU).o
	$(CC) -shared -o $@ $^

vmeio_support.$(CPU).so: vmeio_support.$(CPU).o
	-$(RM) $@
	$(CC) $(CFLAGS) -o $@ -shared $^

vmeio_support.$(CPU).a: vmeio_support.$(CPU).o
	-$(RM) $@
	$(AR) $(ARFLAGS) $@ $^
	$(RANLIB) $@

libencore.$(CPU).a: libencore.$(CPU).o
	-$(RM) $@
	$(AR) $(ARFLAGS) $@ $^
	$(RANLIB) $@

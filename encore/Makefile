
# generated automatically by encore, date 2011-04-24 12:29:24.807801

CPU=L865
KVER=2.6.24.7-rt27
include /acc/dsc/src/co/Make.auto

LIBNAME = librf_vtu
REGFILE = RF_VTU.regs
KERNELSRC=/acc/sys/$(CPU)/usr/src/kernels/$(KVER)
DRIVER_NAME = "vmeio"
DRIVER = vmeio
COMPILE_TIME = $(shell date +%s)
CFLAGS= -g -Wall -I. -I ../../driver
CFLAGS += -I /acc/local/L865/include/
CFLAGS += -DCOMPILE_TIME=$(COMPILE_TIME) -g -Wall -fPIC

MODLIBS = $(LIBNAME).$(CPU).a $(LIBNAME).$(CPU).so
VMELIBS = $(DRIVER)_support.$(CPU).so $(DRIVER)_support.$(CPU).a libencore.$(CPU).o

SRCS=$(LIBNAME).c $(LIBNAME).h



all: libs modules

libs: $(MODLIBS) $(VMELIBS)

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

$(DRIVER)_support.$(CPU).so: $(DRIVER)_support.$(CPU).o
	-$(RM) $@
	$(CC) $(CFLAGS) -o $@ -shared $^

$(DRIVER)_support.$(CPU).a: $(DRIVER)_support.$(CPU).o
	-$(RM) $@
	$(AR) $(ARFLAGS) $@ $^
	$(RANLIB) $@

libencore.$(CPU).a: libencore.$(CPU).o
	-$(RM) $@
	$(AR) $(ARFLAGS) $@ $^
	$(RANLIB) $@

modules: 
	cp Module.symvers.vmebus Module.symvers
	make -C $(KERNELSRC) M=`pwd` KVER=$(KVER) 'DRIVER_NAME=$(DRIVER_NAME)' modules
clean:
	rm -f *.o *.a *.so 
	make -C $(KERNELSRC) M=`pwd` clean



#================================================================
# Makefile to produce cvorb library
#================================================================

CPU=L865

include /ps/dsc/src/co/Make.auto

COMPILE_TIME:=$(shell date +%s)

VMEIO=/acc/src/dsc/drivers/cohtdrep/lewis/vmeio-drivers/vmeio

CFLAGS= -DCOMPILE_TIME=$(COMPILE_TIME) -g -Wall -I. -I../driver -I$(VMEIO)/include -fPIC

all: cvorb_support.$(CPU).so cvorb_support.$(CPU).a

cvorb_support.c: $(VMEIO)/support/vmeio_support.c
	cp $(VMEIO)/support/vmeio_support.c cvorb_support.c

cvorb_support.$(CPU).o: cvorb_support.c

cvorb_support.$(CPU).a: cvorb_support.$(CPU).o
	-$(RM) $@
	$(AR) $(ARFLAGS) $@ $^
	$(RANLIB) $@

cvorb_support.$(CPU).so: cvorb_support.$(CPU).o
	-$(RM) $@
	$(CC) $(CFLAGS) -o $@ -shared $^

clean:
	rm -f cvorb_support.$(CPU).o
	rm -f cvorb_support.$(CPU).a
	rm -f cvorb_support.$(CPU).so
	rm -f cvorb_support.c
	rm -f cvorb_support.h

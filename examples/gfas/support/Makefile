#================================================================
# Makefile to produce acdxing library
#================================================================

CPU=L865

include /ps/dsc/src/co/Make.auto

COMPILE_TIME:=$(shell date +%s)

VMEIO=/acc/src/dsc/drivers/cohtdrep/lewis/vmeio-drivers/vmeio

CFLAGS= -DCOMPILE_TIME=$(COMPILE_TIME) -g -Wall -I. -I../driver -I$(VMEIO)/include -fPIC

all: gfas_support.$(CPU).so gfas_support.$(CPU).a

gfas_support.c: $(VMEIO)/support/vmeio_support.c
	cp $(VMEIO)/support/vmeio_support.c gfas_support.c

gfas_support.$(CPU).o: gfas_support.c

gfas_support.$(CPU).a: gfas_support.$(CPU).o
	-$(RM) $@
	$(AR) $(ARFLAGS) $@ $^
	$(RANLIB) $@

gfas_support.$(CPU).so: gfas_support.$(CPU).o
	-$(RM) $@
	$(CC) $(CFLAGS) -o $@ -shared $^

clean:
	rm -f gfas_support.$(CPU).o
	rm -f gfas_support.$(CPU).a
	rm -f gfas_support.$(CPU).so
	rm -f gfas_support.c
	rm -f gfas_support.h

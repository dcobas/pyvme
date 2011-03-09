#================================================================
# Makefile to produce acdxing library
#================================================================

include /ps/dsc/src/co/Make.auto

COMPILE_TIME:=$(shell date +%s)

CFLAGS= -DCOMPILE_TIME=$(COMPILE_TIME) -g -Wall -I. -I../include -I../driver -fPIC

SRCS=vmeio_support.c vmeio_support.h

all: vmeio_support.$(CPU).so vmeio_support.$(CPU).a

vmeio_support.$(CPU).o: vmeio_support.c

vmeio_support.$(CPU).a: vmeio_support.$(CPU).o
	-$(RM) $@
	$(AR) $(ARFLAGS) $@ $^
	$(RANLIB) $@

vmeio_support.$(CPU).so: vmeio_support.$(CPU).o
	-$(RM) $@
	$(CC) $(CFLAGS) -o $@ -shared $^

clean:
	rm -f vmeio_support.$(CPU).o
	rm -f vmeio_support.$(CPU).a
	rm -f vmeio_support.$(CPU).so

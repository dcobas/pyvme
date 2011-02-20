#================================================================
# Makefile to produce CtcUserDefinedAccess library
#================================================================

CPU=L865

include /ps/dsc/src/co/Make.auto

COMPILE_TIME:=$(shell date +%s)

CFLAGS= -DCOMPILE_TIME=$(COMPILE_TIME) -g -Wall -I. -I../driver -I../../vmeio/include -fPIC

SRCS=CtcUserDefinedAccess.c CtcUserDefinedAccess.h

all: CtcUserDefinedAccess.$(CPU).so CtcUserDefinedAccess.$(CPU).a

CtcUserDefinedAccess.$(CPU).o: CtcUserDefinedAccess.c

CtcUserDefinedAccess.$(CPU).a: CtcUserDefinedAccess.$(CPU).o
	-$(RM) $@
	$(AR) $(ARFLAGS) $@ $^
	$(RANLIB) $@

CtcUserDefinedAccess.$(CPU).so: CtcUserDefinedAccess.$(CPU).o
	-$(RM) $@
	$(CC) $(CFLAGS) -o $@ -shared $^

clean:
	rm -f CtcUserDefinedAccess.$(CPU).o
	rm -f CtcUserDefinedAccess.$(CPU).a
	rm -f CtcUserDefinedAccess.$(CPU).so

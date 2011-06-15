#================================================================
# Makefile to produce vmeio support library test program
#================================================================

CPU=L865

include /acc/dsc/src/co/Make.auto

COMPILE_TIME:=$(shell date '+%s')

CFLAGS= -DCOMPILE_TIME=$(COMPILE_TIME) -g -Wall -fPIC

OBJS = VmeioCmds.$(CPU).o DoCmd.$(CPU).o GetAtoms.$(CPU).o Cmds.$(CPU).o

all: vmeiotest.$(CPU)
vmeiotest.$(CPU): vmeiotest.$(CPU).o vmeio_support.$(CPU).a
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)

vmeio_support.$(CPU).a: vmeio_support.$(CPU).o
	-$(RM) $@
	$(AR) $(ARFLAGS) $@ $^
	$(RANLIB) $@
vmeio_support.$(CPU).o: vmeio_support.c

clean:
	rm -f *.o *.a *.L865

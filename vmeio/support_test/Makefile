#================================================================
# Makefile to produce vmeio support library test program
#================================================================

include /acc/dsc/src/co/Make.auto

COMPILE_TIME:=$(shell date +%s)

CFLAGS = -g -Wall -I. -I../include -I../driver -DCOMPILE_TIME=$(COMPILE_TIME)

LDLIBS= ../support/vmeio_support.L865.a

ALL  = vmeiotest.$(CPU).o vmeiotest.$(CPU)

SRCS = vmeiotest.c VmeioCmds.c DoCmd.c GetAtoms.c Cmds.c

HDRS = Cmds.h

all: $(ALL)

# Run on Workstation only

vmeiotest.$(CPU).o: $(SRCS) $(HDRS)

vmeiotest.$(CPU): vmeiotest.$(CPU).o

clean:
	rm -f *.o *.$(CPU)

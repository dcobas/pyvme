#================================================================
# Makefile to produce vmeio support library test program
#================================================================

CPU=L865
include /acc/dsc/src/co/Make.common

COMPILE_TIME:=$(shell date +%s)
LIB_PATH=../driver

CFLAGS = -g -Wall -I $(LIB_PATH) -DCOMPILE_TIME=$(COMPILE_TIME)

LDLIBS= $(LIB_PATH)/vmeio_support.a

ALL  = vmeiotest.o vmeiotest

SRCS = vmeiotest.c VmeioCmds.c DoCmd.c GetAtoms.c Cmds.c

HDRS = Cmds.h

all: $(ALL)

# Run on Workstation only

vmeiotest.o: $(SRCS) $(HDRS)

vmeiotest: vmeiotest.o

clean:
	rm -f *.o vmeiotest

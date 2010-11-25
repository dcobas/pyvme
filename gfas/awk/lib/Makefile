#================================================================
# Makefile to produce GFAS library from GFAS.regs
#================================================================

CPU=L865

VMEIO=/acc/src/dsc/drivers/cohtdrep/lewis/vmeio-drivers/vmeio/awk/lib

all: GFAS/libgfas.$(CPU).a

genlib.c.awk: $(VMEIO)/genlib.c.awk
	cp $(VMEIO)/genlib.c.awk .

genlib.h.awk: $(VMEIO)/genlib.h.awk
	cp $(VMEIO)/genlib.h.awk .

genmake.awk:  $(VMEIO)/genmake.awk
	cp $(VMEIO)/genmake.awk .

makelib.bat:  $(VMEIO)/makelib.bat
	cp $(VMEIO)/makelib.bat .

GFAS/libgfas.$(CPU).a: GFAS.regs genlib.c.awk genlib.h.awk genmake.awk makelib.bat
	./makelib.bat GFAS

clean:
	rm -f genlib.c.awk
	rm -f genlib.h.awk
	rm -f genmake.awk
	rm -f makelib.bat
	rm -f Makefile.lib

#================================================================
# Makefile to produce vmeio support library test program
#================================================================

CPU=L865

VMEIO = /acc/src/dsc/drivers/cohtdrep/lewis/vmeio-drivers/vmeio/support_test

gfastest.$(CPU): $(VMEIO)/vmeiotest.$(CPU)
	cp $(VMEIO)/vmeiotest.$(CPU) gfastest.$(CPU)
	cp $(VMEIO)/vmeiotest.config .
	cp GFAS.regs vmeio.regs

clean:
	rm -f gfastest.$(CPU)
	rm -f vmeiotest.config
	rm -f vmeio.regs

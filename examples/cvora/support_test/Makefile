#================================================================
# Makefile to produce vmeio support library test program
#================================================================

CPU=L865

VMEIO = /acc/src/dsc/drivers/cohtdrep/lewis/vmeio-drivers/vmeio/support_test

cvorbtest.$(CPU): $(VMEIO)/vmeiotest.$(CPU)
	cp $(VMEIO)/vmeiotest.$(CPU) cvorbtest.$(CPU)
	cp $(VMEIO)/vmeiotest.config .
	cp CVORA.regs vmeio.regs

clean:
	rm -f cvoratest.$(CPU)
	rm -f vmeiotest.config
	rm -f vmeio.regs

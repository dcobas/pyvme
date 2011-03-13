#================================================================
# Makefile to produce GFAS library from GFAS.regs
#================================================================

CPU=L865

VMEIO=/acc/src/dsc/drivers/cohtdrep/lewis/vmeio-drivers/vmeio/awk/regs

all: GFAS.regs

q1.awk:  $(VMEIO)/q1.awk
	cp $(VMEIO)/q1.awk .

q2.awk:  $(VMEIO)/q2.awk
	cp $(VMEIO)/q2.awk .

q3.awk:  $(VMEIO)/q3.awk
	cp $(VMEIO)/q3.awk .

q4.awk:  $(VMEIO)/q4.awk
	cp $(VMEIO)/q4.awk .

q6.awk:  $(VMEIO)/q6.awk
	cp $(VMEIO)/q6.awk .

makeregs.bat:  $(VMEIO)/makeregs.bat
	cp $(VMEIO)/makeregs.bat .

GFASCHANNEL.regs: makeregs.bat q1.awk q2.awk q3.awk q4.awk q6.awk
	./makeregs.bat GFASCHANNEL

GFAS.regs: GFASCHANNEL.regs
	cat GFASCHANNEL.regs | /bin/sed 's/gfaschannel/gfas/g' > GFAS.regs

clean:
	rm -f q1.awk
	rm -f q2.awk
	rm -f q3.awk
	rm -f q4.awk
	rm -f q6.awk
	rm -f makeregs.bat
	rm -f GFASCHANNEL.regs

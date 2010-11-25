#================================================================
# Makefile to install GFAS driver
#================================================================

CPU=L865

VMEIO=/acc/src/dsc/drivers/cohtdrep/lewis/vmeio-drivers/vmeio/awk/install

all: doinstall.bat geninstall.awk makeinstall.bat

doinstall.bat:  $(VMEIO)/doinstall.bat
	cp $(VMEIO)/doinstall.bat .

geninstall.awk:  $(VMEIO)/geninstall.awk
	cp $(VMEIO)/geninstall.awk .

makeinstall.bat:  $(VMEIO)/makeinstall.bat
	cp $(VMEIO)/makeinstall.bat .

clean:
	rm -f doinstall.bat
	rm -f geninstall.awk
	rm -f makeinstall.bat

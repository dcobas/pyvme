CPU=L865

# KVER=2.6.24.7-rt21
# KVER=2.6.24.7-rt27
# KVER=2.6.29.4-rt15
# KVER=2.6.31.6-rt19

KVER=2.6.24.7-rt27

KERNELSRC=/acc/sys/$(CPU)/usr/src/kernels/$(KVER)
DRIVER_NAME = "vmeio"
CFLAGS += -I /acc/local/L865/include/

all: libs modules doc

libs: libvmeio.so

libvmeio.so: libvmeio.o
	$(CC) -shared -o $@ $^

doc:
	pydoc -w ./pyvme.py
	pdflatex brka

modules: 
	cp Module.symvers.vmebus Module.symvers
	make -C $(KERNELSRC) M=`pwd` KVER=$(KVER) 'DRIVER_NAME=$(DRIVER_NAME)' modules
clean:
	rm -f *.o *.a *.pyc *.so \
		brka.aux brka.dvi brka.log
	rm -f pyvme.html
	rm -rf $(CPU)
	make -C $(KERNELSRC) M=`pwd` clean


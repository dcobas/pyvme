CFLAGS += -I/acc/src/dsc/drivers/coht/include -O0
CC = gcc

all: libvmebus.a libvmebus.so pp.so

libvmebus.a: libvmebus.o
	ar r $@ $^
libvmebus.so: libvmebus.o
	$(CC) -shared $^ -o $@
pp.so: pp.o
	$(CC) -shared $^ -o $@

clean:
	rm -f *.o *.a *.so

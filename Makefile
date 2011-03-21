all: driver test
	make -C driver
	make -C test

clean:
	make -C driver clean
	make -C test clean

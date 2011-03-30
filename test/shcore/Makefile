SOURCES=shell.o cmd_core.o cmd_vmeio.o main.o
CFLAGS += -Wall -ggdb -I. -I../../driver -I/acc/src/dsc/drivers/coht/include -I./vmeio

VPATH+=./vmeio

all: vmeiosh

vmeiosh: $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) ../../driver/vmeio_support.a $(LDFLAGS) -o vmeiosh

clean:
	rm -rf $(SOURCES) vmeiosh

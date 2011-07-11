DOXY_INSTPATH = /acc/doc/html/private/coht/doxy/lib/vmeio

all:
	echo Make documentation for vmeio support library
	rm -f ,*.h
	rm -rf doc
	doxy/doxy.sh -n"Vmeio support library API" -o "doc" ../../include/vmeio_support.h ../../include/vmeio.h

doxy_install: all
	mkdir -p $(DOXY_INSTPATH)
	cp -r doc/html/* $(DOXY_INSTPATH)

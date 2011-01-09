#!	/bin/sh
insmod vmeio.ko \
        vme1=0x800  amd1=0x29 win1=0x400 dwd1=4 \
	vme2=0x8100 amd2=0x29 win2=0x100 dwd2=2 \
	vecs=0x83 lvls=2 luns=0

MAJOR=` grep vmeio /proc/devices | awk '{ print $1 }' `
mknod /dev/vmeio.0 c $MAJOR 0

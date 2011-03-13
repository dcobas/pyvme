#!	/bin/sh

LUNS="0"

insmod vmeio.ko \
        vme1=0x6000  amd1=0x29 win1=0x800 dwd1=2 \
 	vecs=0xA6 lvls=3 luns=$LUNS

MAJOR=` grep vmeio /proc/devices | awk '{ print $1 }' `

for lun in $LUNS; do
    DEVICE=/dev/vmeio.$lun
    if [ -e $DEVICE ] ; then
	    rm -f $DEVICE
    fi
    mknod $DEVICE c $MAJOR 0
done

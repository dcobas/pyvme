#!	/bin/sh

DRIVER_NAME=vmeio
INSMOD="insmod vmeio.ko lun=0 \
	base_address1=0x10000000 size1=0x400000 data_width1=32 am1=0x09 \
	base_address2=0x8000000  size2=0x800000 data_width2=32 am2=0x09"

# /sbin/insmod vmeio.ko luns=0 vme1=0x20000000 lvls=2 vecs=0xc9 amd1=0x9 win1=0x800000 dwd1=4
# /sbin/insmod vmeio.ko luns=0 vme1=0x100000 amd1=0x39 win1=0x80000 dwd1=4
# /sbin/insmod vmeio.ko luns=0 vme1=0x6200 amd1=0x29 win1=0x200 dwd1=2 lvls=2 vecs=125

echo "installing driver module $DRIVER_NAME"
sh -c "$INSMOD"

MAJOR=`cat /proc/devices | awk '$2 == "vmeio" {print $1}'`
MINORS=0
if [ -z "$MAJOR" ]; then
	echo "driver $DRIVER_NAME not installed!"
	exit 1
fi

echo "creating device nodes for device major $MAJOR"
for MINOR in $MINORS; do
    rm -f /dev/$DRIVER_NAME.$MINOR
    mknod /dev/$DRIVER_NAME.$MINOR c $MAJOR $MINOR
done


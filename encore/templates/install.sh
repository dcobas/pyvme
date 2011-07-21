#!/bin/sh

# Generated automatically by encore at %(date)s"
echo "Installing %(device_name)s driver..."

INSMOD_ARGS=`awk -f transfer2insmod.awk %(device_name)s %(transfer)s`

if [ x"$INSMOD_ARGS" == x"" ] ; then
    echo "No %(device_name)s declared in %(transfer)s, exiting"
    exit 1
fi

INSMOD_CMD="insmod %(driver_name)s.ko $INSMOD_ARGS"
echo "installing %(driver_name)s by insmod %(driver_name)s $INSMOD_ARGS"
sh -c "$INSMOD_CMD"

MAJOR=`cat /proc/devices | awk '$2 == "%(driver_name)s" {print $1}'`
if [ -z "$MAJOR" ]; then
	echo "driver %(driver_name)s not installed!"
	exit 1
fi

MINORS=`awk '/^#\+#/ && $6 == "%(device_name)s" { printf("%%s ", $7) }' %(transfer)s`
echo "creating device nodes for driver %(driver_name)s, major $MAJOR, minors $MINORS"
for MINOR in $MINORS; do
    rm -f /dev/%(driver_name)s.$MINOR
    mknod /dev/%(driver_name)s.$MINOR c $MAJOR $MINOR
done

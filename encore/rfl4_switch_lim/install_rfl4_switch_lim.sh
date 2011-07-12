#!/bin/sh

# Generated automatically by encore at 2011-07-12 12:08:15.210834"
echo "Installing RFL4_SWITCH_LIM driver..."

INSMOD_ARGS=`awk -f transfer2insmod.awk RFL4_SWITCH_LIM /etc/transfer.ref`

if [ x"$INSMOD_ARGS" == x"" ] ; then
    echo "No RFL4_SWITCH_LIM declared in /etc/transfer.ref, exiting"
    exit 1
fi

INSMOD_CMD="insmod rfl4_switch_lim.ko $INSMOD_ARGS"
echo "installing rfl4_switch_lim by insmod rfl4_switch_lim $INSMOD_ARGS"
sh -c "$INSMOD_CMD"

MAJOR=`cat /proc/devices | awk '$2 == "rfl4_switch_lim" {print $1}'`
if [ -z "$MAJOR" ]; then
	echo "driver rfl4_switch_lim not installed!"
	exit 1
fi

MINORS=`awk '/^#\+#/ && $6 == "RFL4_SWITCH_LIM" { printf("%s", $7) }' /etc/transfer.ref`
echo "creating device nodes for driver rfl4_switch_lim, major $MAJOR, minors $MINORS"
for MINOR in $MINORS; do
    rm -f /dev/rfl4_switch_lim.$MINOR
    mknod /dev/rfl4_switch_lim.$MINOR c $MAJOR $MINOR
done

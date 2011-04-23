import sys
import datetime

instprog_template = r'''#!/bin/sh

# Generated automatically by install.py at %(date)s"
echo "Installing %(device_name)s driver..."

INSMOD_ARGS=`awk -f transfer2insmod.awk %(device_name)s %(transfer)s`

if [ x"$INSMOD_ARGS" == x"luns=" ] ; then
    echo "No %(device_name)s declared in %(transfer)s, exiting"
    exit 1
fi

echo "installing %(driver_name)s by insmod $INSMOD_ARGS"
insmod %(driver_name)s.ko "$INSMOD_ARGS"

MAJOR=`cat /proc/devices | awk '$2 == "%(driver_name)s" {print $1}' %(transfer)s`
if [ -z "$MAJOR" ]; then
	echo "driver %(driver_name)s not installed!"
	exit 1
fi

MINORS=`awk '/^#+#/ && $6 == "%(device_name)s" { print $7, }' %(transfer)s`
echo "creating device nodes for driver %(driver_name)s, major $MAJOR, minors $MINORS"
for MINOR in $MINORS; do
    rm -f /dev/%(driver_name)s.$MINOR
    mknod /dev/%(driver_name)s.$MINOR c $MAJOR $MINOR
done
'''

def gen_install_script(device_name, driver_name,
                    transfer='/etc/transfer.ref'):

    date = datetime.datetime.now().isoformat(' ')

    return instprog_template % locals()
    

if __name__ == '__main__':
    if len(sys.argv) < 2:
        print 'usage: install.py DEVICE_NAME'
        sys.exit(1)

    device_name = sys.argv[1].upper()
    driver_name = device_name.lower()
    transfer = '/etc/transfer.ref'
    print gen_install_script(device_name, driver_name, transfer)

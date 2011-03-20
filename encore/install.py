import sys
import datetime

instprog_template = r'''#!/bin/sh

echo "Generated automatically by geninstall.awk at %(date)s"
echo "Installing %(device_name)s driver..."

INSMOD_ARGS=`awk -f transfer2insmod.awk %(device_name)s %(transfer)s`

echo "installing %(driver_name)s by insmod $INSMOD_ARGS"
insmod "$INSMOD_ARGS"

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
    device_name = sys.argv[1].upper()
    driver_name = sys.argv[1].lower()
    transfer = '/acc/dsc/tst/cfv-864-cdv28/etc/transfer.ref'

    return instprog_template % locals()
    
    dummies = {
        'date'			    : '0xa5a5a5',
        'device_name'		: '0xa5a5a5',
        'driver_name'		: '0xa5a5a5',
        'transfer'  		: '0xa5a5a5',
        'luns_list'			: '0xa5a5a5',
        'base_address_list1': '0xa5a5a5',
        'base_address_list2': '0xa5a5a5',
        'size1'			    : '0xa5a5a5',
        'size2'		    	: '0xa5a5a5',
        'data_width1'		: '0xa5a5a5',
        'data_width2'		: '0xa5a5a5',
        'am1'			    : '0xa5a5a5',
        'am2'		    	: '0xa5a5a5',
        'level'		    	: '0xa5a5a5',
        'vectors_list'    	: '0xa5a5a5',
}

print gen_install_script('RF_VTU', 'rf_vtu')

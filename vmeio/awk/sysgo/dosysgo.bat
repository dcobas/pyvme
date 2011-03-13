#!/bin/sh
echo
echo Build and execute sysgo for each installed $1 lun

rm -f /tmp/sysgo.$1.awk
rm -f /tmp/sysgo.$1
cat callsysgo.awk | /bin/sed 's/NAME/'$1'/g' > /tmp/sysgo.$1.awk
chmod 774 /tmp/sysgo.$1.awk

cat /etc/transfer.ref | /tmp/sysgo.$1.awk > /tmp/sysgo.$1
chmod 774 /tmp/sysgo.$1

/tmp/sysgo.$1

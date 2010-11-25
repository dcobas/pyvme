#!/bin/sh
echo
echo Build and instal $1 from transfer.ref
echo
rm -f /tmp/install.$1.awk
rm -f /tmp/install.$1
cat geninstall.awk | /bin/sed 's/NAME/'$1'/g' > /tmp/install.$1.awk
chmod 774 /tmp/install.$1.awk

# User the real transfer.ref in /etc
cat /etc/transfer.ref | /tmp/install.$1.awk > /tmp/install.$1
chmod 774 /tmp/install.$1

echo
echo Installing $1 driver
echo
/tmp/install.$1

echo
echo End $1 installation

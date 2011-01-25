#!/bin/bash

if [ /acc/dsc/$2/$3/etc/transfer.ref ]
then
rm -f ./transfer.ref
cp /acc/dsc/$2/$3/etc/transfer.ref .
fi

echo
echo Building installer file for $1 from transfer.ref
echo

rm -f /tmp/install.$1.awk
rm -f /tmp/install.$1
cat geninstall.awk | /bin/sed 's/NAME/'$1'/g' > /tmp/install.$1.awk
chmod 774 /tmp/install.$1.awk

# For tests just use a local copy of transfer.ref
cat ./transfer.ref | /tmp/install.$1.awk > /tmp/install.$1

chmod 774 /tmp/install.$1

#Copy the installer script here
cp /tmp/install.$1 .

echo
echo Installer script for $1 built
echo


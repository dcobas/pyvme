#!/bin/bash

if [ /acc/dsc/$2/$3/etc/transfer.ref ]
then
rm -f ./transfer.ref
cp /acc/dsc/$2/$3/etc/transfer.ref .
fi

echo
echo Building installer file for $1 from transfer.ref
echo

# For tests just use a local copy of transfer.ref
cat ./transfer.ref | awk -f geninstall.awk $1 > ./install.$1
chmod 744 ./install.$1

echo
echo Installer script for $1 built
echo


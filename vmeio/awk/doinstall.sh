#!/bin/sh
echo
echo Installing $1 driver
echo
cat /etc/transfer.ref | awk -f geninstall.awk $1 | sh -x
echo
echo End $1 installation

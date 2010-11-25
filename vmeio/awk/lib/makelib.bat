#!/bin/sh

echo $1 | ./genmake.awk > Makefile.lib
make -f Makefile.lib clean
make -f Makefile.lib all

if [ $1 ]
then
   rm -rf $1
fi

mkdir $1
mv *.[ch] $1
mv *L865* $1

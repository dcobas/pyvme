#!/bin/sh

if [ $1 ]
then

   if [ $2 ]
   then
      echo $1 " " $2  | ./genmake.awk > Makefile.lib
   else
      echo $1 " L865" | ./genmake.awk > Makefile.lib
   fi

   make -f Makefile.lib clean
   make -f Makefile.lib all

   rm -rf $1

   mkdir $1
   mv *.[ch] $1
   mv *L865* $1

else

   cat README

fi

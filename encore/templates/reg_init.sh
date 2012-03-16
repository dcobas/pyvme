#!	/bin/sh
#
#  interrupt register initialization
#
#  RF boards typically contain a conventional VECTOR and LEVEL
#  register initialized to the interrupt vector and level the board
#  should use
#
#  this init script sets the values of these registers from the
#  transfer.ref description of the boards to install
#

awk -f transfer2init.awk $1 $2 | sh

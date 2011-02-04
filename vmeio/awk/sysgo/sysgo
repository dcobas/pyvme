#!/bin/sh

# Arg $1 is the driver name
# Arg $2 is the logical unit number
# Arg $3 is the interrupt level
# Arg $4 is the interrupt vector

echo "sysgo name=" $1 " lun=" $2 " lvl=" $3 " vec=" $4

# You can pipe commands to the test program to initialize and setup
# registers in your hardware. For example if there were registers named
# LEVEL and VECTOR that needed to be set to the interrupt level and vector
# the following command would do the trick. Notice the test program name
# is test$1 I.E. if the driver name is vd80 then test$1 expands to testvd80.

# echo "win 1 mod " $2 " (ereg LEVEL " $3 ") (ereg VECTOR " $4 ") q" | test$1

# Here you can put what ever stuff you want
# Just pipe the commands to the test program as above

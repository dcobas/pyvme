#!/bin/sh
echo $1 | ./q1.awk | sqlplus copub/co | ./q2.awk | ./q3.awk | sqlplus copub/co > /tmp/t1
echo $1 | ./q1.awk | sqlplus copub/co | ./q2.awk | ./q4.awk > /tmp/q5.awk
chmod 777 /tmp/q5.awk
cat /tmp/t1 | /tmp/q5.awk | ./q6.awk > $1.regs

echo
echo Listing of $1.regs follows
echo
cat $1.regs
echo

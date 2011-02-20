#!   /bin/sh

( sqlplus -s copub/co@dbabco <<QUERY
set echo off;
set verify off;
set feed off;
set pagesize 0;
set pause off;
set linesize 4000;
set trims on;
select name, rwmode, mr.offsetval, wordsize, depthval, block, 
	nvl(mr.offset,0) as r_offset,
	nvl(mb.offsetval,0) as b_offsetval
from moduleregisters mr join moduleblocks mb 
	using (block, moduletype_id)
where moduletype_id = ( 
	select hwtype_id 
	from hard_types 
	where hwtype = '$1' )
order by block, b_offsetval, r_offset;
QUERY
) | awk 'BEGIN {
	print "NAME modname"
	print
	print "  RegName\t\tRwFlags\tOffset\t\tSize\tWindow\tDepth"
	print

	to_size["long"]  = 4
	to_size["short"] = 2
	to_size["char"]  = 1
	type["rw"]  = "RW"
	type["rc"]  = "RO"
	type["rc"]  = "RO"
	type["r"]   = "RO"
	type["w"]   = "RW"
	type["rw"]  = "RW"
	type["e"]   = "WO"
	type["rc"]  = "RO"
	type["rwc"] = "RW"
}

{
	printf("$ %-16s\t%2s\t0x%08X\t%d\t1\t%s\n",
		$1, type[$2], $3 + $8, to_size[$4], $5);
}' | /bin/sed 's/modname/'$1'/g' > $1.regs
if (( $# > 1 )); then
   mv $1.regs /tmp
   cat /tmp/$1.regs | ./splitwins.bat $2 > $1.regs
fi

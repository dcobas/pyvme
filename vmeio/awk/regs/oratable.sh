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
	nvl(MB.OFFSETVAL,0) as b_offsetval
from moduleregisters mr join moduleblocks mb 
	using (block, moduletype_id)
where moduletype_id = ( 
	select hwtype_id 
	from hard_types 
	where hwtype = '$1' )
order by block, b_offsetval, r_offset;
QUERY
) | awk '{
	if ($2 == "r" || $2 == "rc")
		$2 = "RO"
	else
		$2 = "RW"
	$3 = $3 + $8
	if ($4 == "long")
		$4 = 4
	else if ($4 == "short")
		$4 = 2
	else
		$4 = 1
	printf("$ %-16s\t%2s\t0x%08X\t%d\t1\t%s\t0x%08X\n",
		$1, $2, $3, $4, $5, $8);
	}'


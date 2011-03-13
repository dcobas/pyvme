#!   /bin/sh

#
#  this SQL query extracts from the CCDB the list of registers
#  concerning the HWTYPE (module name) given to it as a parameter
#
#  Each returned line contains information for a register:
#  
#	name			register name
#	rwmode			one of NULL, r, w, rw, rc, rwc or e
#	block			block number (from 0)
#	block_address_space	1 or 2
#	block_offset		block offset within its address space
#	register_offset		register offset within block
#	total_offset		total register offset within address space
#	wordsize		wordsize of register
#	depth			number of words of register
#
#  WARNING: if no argument is passed, or the argument does not describe
#  an existing module, an empty list is returned without further notice
#

sqlplus -s copub/co@dbabco <<QUERY
set echo off;
set verify off;
set feed off;
set pagesize 0;
set pause off;
set linesize 4000;
set trims on;
select name, 
	rwmode, 
	block, 
	mb.address as block_address_space,
	nvl(mb.offsetval,0) as block_offsetval,
	mr.offsetval as register_offset,
	mr.offsetval + nvl(mb.offsetval,0) as total_offset,
	wordsize, 
	depthval as depth
from moduleregisters mr join moduleblocks mb 
	using (block, moduletype_id)
where moduletype_id = ( 
	select hwtype_id 
	from hard_types 
	where hwtype = '$1' )
order by block, block_offsetval, register_offset;
QUERY

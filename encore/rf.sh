#!	/bin/sh

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
    depthval as depth,
    '"' || nvl(mr.description, '') || '"' as description
from moduleregisters mr join moduleblocks mb
    using (block, moduletype_id)
where moduletype_id = (
    select hwtype_id
    from hard_types
    where hwtype = '$1' )
order by block, block_offsetval, register_offset;
QUERY

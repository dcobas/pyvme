#!/bin/awk -f

BEGIN { }

{
   print "set linesize 256;"
   print "select MODULETYPE_ID, NAME, RWMODE, MR.OFFSETVAL, WORDSIZE, DEPTHVAL, BLOCK, MR.OFFSET r_offset, nvl(MB.OFFSET,0) b_offset "
   print "from MODULEREGISTERS MR "
   print "join MODULEBLOCKS MB using (BLOCK, MODULETYPE_ID) "
   print "where MODULETYPE_ID = " $1 " "
   print "order by block, b_offset, r_offset;"
}

END { }







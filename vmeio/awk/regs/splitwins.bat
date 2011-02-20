#!/bin/sh
awk '
BEGIN {  splt=strtonum('$1');
}

{ if ($1 == "NAME") {
	print "NAME " $2
	print ""
	print "  RegName\t\tRwFlags\tOffset\t\tSize\tWindow\tDepth"
	print ""
   }

   if ($1 == "$") {
      printf("$ %-16s\t%2s\t",$2,$3);
      offs = strtonum($4);
      if ((splt > 0) && (offs >= splt)) {
	 offs = offs - splt;
	 wind = 2;
      } else {
	 wind = 1;
      }
      printf("0x%08X\t%d\t%d\t%s\n",offs,$5,wind,$7);
   }
}

END { }
'

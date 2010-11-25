#!/bin/awk -f
BEGIN { }

{
   if ($1 == "NAME") {
      print $1 " " tolower($2);
      print ""
      print "  RegName\t\tRwFlags\tOffset\t\tSize\tWindow\tDepth"
      print ""
   } else {

      printf("$ %16s ",$2);
      printf("\t");

      if ($3 == "w") printf("WO ");
      else if (($3 == "r" ) || ($3 == "rc")) printf("RO ");
      else printf("RW ");
      printf("\t");

      printf("0x%08X ", $4 + strtonum("0x" substr($9,3,length($9))));
      printf("\t");

      if ($5 == "long") printf("4 ");
      else if ($5 == "short") printf("2 ");
      else printf("1 ");
      printf("\t");

      printf("1 ");
      printf("\t");

      if ($6 > 0) printf("%s ", $6);
      else        printf("1 ");
      printf("\t");

      if (NF == 9) printf("\n");
      else printf("#Error fields:%d expected:9\n",NF);

   }
}

END { }

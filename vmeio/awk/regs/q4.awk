#!/bin/awk -f

BEGIN { print "#!/bin/awk -f";
	print "";
	pname = 1;
      }

{
   if (pname==1) {
      pname = 0;
      print "BEGIN { print \"NAME " $2 "\"}";
   }
}

{ print "{ if ($1 == " $1 ") print $1 \" \" $2 \" \" $3 \" \" $4 \" \" $5 \" \" $6 \" \" $7 \" \" $8 \" \" $9; }" };

END { print "";
      print "END {}";
    }

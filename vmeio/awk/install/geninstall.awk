#!/bin/awk -f

BEGIN {

   print ""
   lun = 0;

   format = "%Y %a %B-%e %H:%M:%S"
   print "#!/bin/sh"
   print "echo Generated automatically by geninstall.awk at:" strftime(format)
   print "echo Installing " toupper("NAME") " driver..."
   print "echo 8 > /proc/sys/kernel/printk"
   print ""
}

{
   if ($1 == "#+#") {
      if ($6 == toupper("NAME")) {

	 luns[lun] = $7;
	 vme1[lun] = $11;
	 vme2[lun] = $16;

	 if ($9 == "ST") amd1[lun] = "0x39";
	 if ($9 == "SH") amd1[lun] = "0x29";
	 if ($9 == "CR") amd1[lun] = "0x2F";

	 if ($14 == "ST") amd2[lun] = "0x39";
	 if ($14 == "SH") amd2[lun] = "0x29";
	 if ($14 == "CR") amd2[lun] = "0x2F";

	 if ($10 == "DP16") dwd1[lun] = "2";
	 if ($10 == "DP32") dwd1[lun] = "4";

	 if ($15 == "DP16") dwd2[lun] = "2";
	 if ($15 == "DP32") dwd2[lun] = "4";

	 win1[lun] = $12;
	 win2[lun] = $17;

	 if (NF >= 23) {
	    lvl[lun] = strtonum($22);
	    vec[lun] = strtonum($23);
	 } else {
	    lvl[lun] = 0;
	    vec[lun] = 0;
	 }
	 lun++;
      }
   }
}

END {
   printf "/sbin/insmod " tolower("NAME") ".ko dname=\"" tolower("NAME") "\""

   printf " luns="
   for (i=0; i<lun; i++) {
      printf luns[i]
      if (i < lun -1) printf ","
   }

   printf " vme1="
   for (i=0; i<lun; i++) {
      printf "0x" vme1[i]
      if (i < lun -1) printf ","
   }

   printf " win1=0x" win1[0]
   printf " amd1="   amd1[0]
   printf " dwd1="   dwd1[0]

   if (vme2[0] != 0) {
      printf " vme2="
      for (i=0; i<lun; i++) {
	 printf "0x" vme2[i]
	 if (i < lun -1) printf ","
      }

      printf " win2=0x" win2[0]
      printf " amd2="   amd2[0]
      printf " dwd2="   dwd2[0]
   }

   if ((lvl[0] != 0) && (vec[0] != 0)) {
      printf " lvls="
      for (i=0; i<lun; i++) {
	 printf lvl[i]
	 if (i < lun -1) printf ","
      }
      printf " vecs="
      for (i=0; i<lun; i++) {
	 printf "0x%X",vec[i]
	 if (i < lun -1) printf ","
      }
   }

   print ""

   print "MAJOR=`cat /proc/devices | awk '{if ($2 == \"" tolower("NAME") "\") printf $1}'`"
   print "echo Making nodes for ${MODULE} major device " tolower("NAME") " ${MAJOR}"
   print "if [ -z \"$MAJOR\" ]; then"
   print "     echo \"driver " toupper("NAME") " not installed !\""
   print "     exit 2"
   print "fi"
   print ""

   print "rm -f /dev/" tolower("NAME") ".*"
   for (i=0; i<lun; i++) {
      print "/bin/mknod  -m 0666 /dev/" tolower("NAME") "." luns[i] " c ${MAJOR} " i
   }
   print ""
}

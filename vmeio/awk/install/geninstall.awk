#!/bin/awk -f

BEGIN {

   lun = 0;

   /* first argument is driver name */
   name = ARGV[1]
   delete ARGV[1]
   lowername = tolower(name)
   uppername = toupper(name)

   /* xlate symbolic am and dw as seen in ccdb to numbers */
   am["EX"] = "0x9"
   am["SH"] = "0x39"
   am["ST"] = "0x29"
   am["CR"] = "0x2F"
   datawidth["DP16"] = "2"
   datawidth["DP32"] = "4"

   format = "%Y %a %B-%e %H:%M:%S"
   print "#!/bin/sh"
   print "echo Generated automatically by geninstall.awk at:" strftime(format)
   print "echo Installing " uppername " driver..."
   print "echo 8 > /proc/sys/kernel/printk"
   print ""
}

{
   if ($1 == "#+#") {
      if ($6 == uppername) {

	 luns[lun] = $7;
	 vme1[lun] = $11;
	 vme2[lun] = $16;

	 amd1[lun] = am[$9]
	 amd2[lun] = am[$14]

	dwd1[lun] = datawidth[$10]
	dwd2[lun] = datawidth[$15]

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
   printf "/sbin/insmod " lowername ".ko dname=\"" lowername "\""

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

   print "MAJOR=`cat /proc/devices | awk '{if ($2 == \"" lowername "\") printf $1}'`"
   print "echo Making nodes for ${MODULE} major device " lowername " ${MAJOR}"
   print "if [ -z \"$MAJOR\" ]; then"
   print "     echo \"driver " uppername " not installed !\""
   print "     exit 2"
   print "fi"
   print ""

   print "rm -f /dev/" lowername ".*"
   for (i=0; i<lun; i++) {
      print "/bin/mknod  -m 0666 /dev/" lowername "." luns[i] " c ${MAJOR} " i
   }
   print ""
}

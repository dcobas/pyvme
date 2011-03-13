#!/bin/awk -f

BEGIN {
   lun = 0;
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
   for (i=0; i<lun; i++) {
      printf "./sysgo " tolower("NAME") " " luns[i] " " lvl[i] " " vec[i] "\n"
   }
}

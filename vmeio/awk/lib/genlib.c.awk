#!/bin/awk -f

BEGIN {
   print "#include <vmeio_support.h>\n"
}

{
   if ($1 == "NAME") {
      format = "%Y %a %B-%e %H:%M:%S";
      print("");
      print("/**");
      print(" * Library for " toupper($2) " module ");
      print(" * Generated automatically by genlib.c.awk at:" strftime(format));
      print(" */");
      print("");
      print("#include <lib" tolower($2) ".h>");
      print("");
   }

   if ($1 == "$") {
      print ""

      if ($3 == "RW") { loops = 2; set = 1; }
      if ($3 == "RO") { loops = 1; set = 0; }
      if ($3 == "WO") { loops = 1; set = 1; }

      for (i=0; i<loops; i++) {
	 if (set == 1) {
	    printf("%s","int SET_");
	 } else {
	    printf("%s","int GET_");
	 }
	 if ($7 <= 1) {
	    if ($5 == "8") print $2 "(void *handle, long *" tolower($2) ") {"
	    if ($5 == "4") print $2 "(void *handle, int *" tolower($2) ") {"
	    if ($5 == "2") print $2 "(void *handle, short *" tolower($2) ") {"
	    if ($5 == "1") print $2 "(void *handle, char *" tolower($2) ") {"
	    print("   int offset = 0;");
	 } else {
	    if ($5 == "8") print $2 "(void *handle, int offset, long *" tolower($2) ") {"
	    if ($5 == "4") print $2 "(void *handle, int offset, int *" tolower($2) ") {"
	    if ($5 == "2") print $2 "(void *handle, int offset, short *" tolower($2) ") {"
	    if ($5 == "1") print $2 "(void *handle, int offset, char *" tolower($2) ") {"
	 }
	 print("   struct vmeio_riob_s riob;");
	 print("   riob.winum  =" $6 ";");
	 print("   riob.offset =" $4 " + (offset * "$5");");
	 print("   riob.bsize  =" $5 ";");
	 print("   riob.buffer =" tolower($2) ";");
	 print("   return RAW(handle,&riob," set ");");
	 print("}");
	 print("");
	 set = 0;
      }
   }
}

END {
   print ""
}

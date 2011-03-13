#!/bin/awk -f

BEGIN {
   print "#include <vmeio_support.h>\n"
   print "#ifndef GEN_LIB_API"
   print "#define GEN_LIB_API"
}

{
   if ($1 == "NAME") {
      format = "%Y %a %B-%e %H:%M:%S";
      print("");
      print("/**");
      print(" * API for " toupper($2) " module ");
      print(" * Generated automatically by genlib.h.awk at:" strftime(format));
      print(" */");
   }

   if ($1 == "$") {
      print ""
      print "#define OFFSET_" $2 " (" $4 ")"

      if ($3 == "RW") { loops = 2; set = 1; }
      if ($3 == "RO") { loops = 1; set = 0; }
      if ($3 == "WO") { loops = 1; set = 1; }

      for (i=0; i<loops; i++) {
	 if (set == 1) {
	    print "#define SET_" $2, "DRV_SYMB(_set_" tolower($2) ")"
	    printf("%s","int SET_");
	    set = 0;
	 } else {
	    print "#define GET_" $2, "DRV_SYMB(_get_" tolower($2) ")"
	    printf("%s","int GET_");
	 }
	 if ($7 <= 1) {
	    if ($5 == "8") print $2 "(void *handle, long *" tolower($2) ");"
	    if ($5 == "4") print $2 "(void *handle, int *" tolower($2) ");"
	    if ($5 == "2") print $2 "(void *handle, short *" tolower($2) ");"
	    if ($5 == "1") print $2 "(void *handle, char *" tolower($2) ");"
	 } else {
	    if ($5 == "8") print $2 "(void *handle, int offset, long *" tolower($2) ");"
	    if ($5 == "4") print $2 "(void *handle, int offset, int *" tolower($2) ");"
	    if ($5 == "2") print $2 "(void *handle, int offset, short *" tolower($2) ");"
	    if ($5 == "1") print $2 "(void *handle, int offset, char *" tolower($2) ");"
	 }
      }
   }
}

END {
   print ""
   print "#endif"
}

#!/bin/awk -f

BEGIN {
   format = "%Y %a %B-%e %H:%M:%S";
   print ""
   print "#This makefile was generated automatically by genmake.awk at:" strftime(format)
   print ""
}

{
   print "LIBNAME = lib" tolower($1)
   print "REGFILE = " toupper($1) ".regs"
   print ""
   print "CPU=" $2
   print ""
   print "include /ps/dsc/src/co/Make.auto"
   print ""
   print "CFLAGS= -g -Wall -I. -I../../driver -I/acc/src/dsc/drivers/cohtdrep/lewis/vmeio-drivers/vmeio/include"
   print ""
   print "SRCS=$(LIBNAME).c $(LIBNAME).h $(REGFILE)"
   print ""
   print "all: $(LIBNAME).$(CPU).a $(LIBNAME).$(CPU).so"
   print ""
   print "$(LIBNAME).$(CPU).o: $(SRCS)"
   print ""
   print "$(LIBNAME).h: $(REGFILE) genlib.h.awk"
   print "\tcat $(REGFILE) | ./genlib.h.awk > $(LIBNAME).h"
   print ""
   print "$(LIBNAME).c: $(REGFILE) genlib.c.awk"
   print "\tcat $(REGFILE) | ./genlib.c.awk > $(LIBNAME).c"
   print ""
   print "$(LIBNAME).$(CPU).a: $(LIBNAME).$(CPU).o"
   print "\t-$(RM) $@"
   print "\t$(AR) $(ARFLAGS) $@ $^"
   print "\t$(RANLIB) $@"
   print ""
   print "$(LIBNAME).$(CPU).so: $(LIBNAME).$(CPU).o"
   print "\t-$(RM) $@"
   print "\t$(CC) $(CFLAGS) -o $@ -shared $^"
   print ""
   print "clean:"
   print "\trm -f $(LIBNAME).$(CPU).o"
   print "\trm -f $(LIBNAME).$(CPU).a"
   print "\trm -f $(LIBNAME).$(CPU).so"
   print "\trm -f $(LIBNAME).h"
   print "\trm -f $(LIBNAME).c"
   print ""
}

END { }

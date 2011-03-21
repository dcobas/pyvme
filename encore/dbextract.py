#!   /usr/bin/env	python
#    coding: utf8

import sys
import os
import errno
import stat
import datetime
import csv
from subprocess import Popen, PIPE
from os.path import join
from optparse import OptionParser

field_list = [
    'name',
    'rwmode',
    'block',
    'block_address_space',
    'block_offset',
    'register_offset',
    'total_offset',
    'wordsize',
    'depth',
    ]

query_template = '''
sqlplus -s copub/co@dbabco <<QUERY
set echo off;
set verify off;
set feed off;
set pagesize 0;
set pause off;
set linesize 4000;
set trims on;
select name,
    rwmode,
    block,
    mb.address as block_address_space,
    nvl(mb.offsetval,0) as block_offsetval,
    mr.offsetval as register_offset,
    mr.offsetval + nvl(mb.offsetval,0) as total_offset,
    wordsize,
    depthval as depth
from moduleregisters mr join moduleblocks mb
    using (block, moduletype_id)
where moduletype_id = (
    select hwtype_id
    from hard_types
    where hwtype = '%s' )
order by block, block_offsetval, register_offset;
QUERY'''

def get_register_data(module_name):
    """get a list of dicts containing register attributes"""

    query = query_template % module_name
    pipe =  Popen([query], shell=True, stdout=PIPE)
    out = pipe.communicate()[0]
    return [ dict(zip(field_list, line.split()))
            for line in out.split('\n')
            if line ]

def gen_plain_file(register_list, filename):
    """given a list of dicts with registers, construct a columnated file"""

    out = open(filename, 'wb')
    for reg in register_list:
        for key in field_list:
            out.write('%s\t' % reg[key])
        out.write('\n')
    out.close()

def gen_csv_file(register_list, filename):
    """given a list of dicts with registers, construct a CSV file"""

    out = csv.DictWriter(open(filename, 'wb'), field_list)
    for reg in register_list:
        out.writerow(reg)

def gen_offsets(register_list):
    """generate offset list #define's"""

    define_template = "#define\t%(offset_name)-16s\t0x%(offset)08x\n"

    defines = []
    for register in register_list:
        name = register['name']
        offset = int(register['total_offset'])
        offset_name = 'OFFSET_%s' % name
        define = define_template % locals()
        defines.append(define)
    return ''.join(defines)

def gen_lib_decls(register_list, prefix="vmeio"):
    """produce declarations for the get/set user lib functions"""
    decl_template = """
int %(driver_name)s_set_%(register_name)s (struct __vsl_device *handle, %(argtype)s value);
int %(driver_name)s_get_%(register_name)s (struct __vsl_device *handle, %(argtype)s *value);"""
    decls = []
    for register in register_list:
        driver_name = prefix
        register_name = register['name']
        argtype = register['wordsize']
        decl = decl_template % locals()
        decls.append(decl)
    return ''.join(decls)

def gen_lib_calls(register_list, prefix="vmeio"):
    """produce code for the get/set user lib functions"""
    call_template = """
int %(driver_name)s_set_%(register_name)s(struct __vsl_device *handle, %(argtype)s value) {
    return usr_raw_write(%(offset_name)s, value);
}

int %(driver_name)s_get_%(register_name)s(struct __vsl_device *handle, %(argtype)s *value) {
    return usr_raw_read(%(offset_name)s, *value);
}
"""
    calls = []
    for register in register_list:
        driver_name = prefix
        register_name = register['name']
        argtype = register['wordsize']
        offset_name = 'OFFSET_%s' % register_name
        call = call_template % locals()
        calls.append(call)
    return ''.join(calls)

def gen_h_file(register_list, libname, filename):
    """produce a lib<module>.h user library header"""
    h_file_header = """#include "vmeio_support.h"\n\n"""

    out = open(filename, 'wb')
    out.write(h_file_header)
    out.write(gen_offsets(register_list))
    out.write('\n')
    out.write(gen_lib_decls(register_list))
    out.close()

def gen_c_file(register_list, libname, filename):
    """produce a lib<module>.c user library module"""
    c_file_header = """#include "%s.h"\n\n""" % libname

    out = open(filename, 'wb')
    out.write(c_file_header)
    out.write(gen_lib_calls(register_list))
    out.close()

def gen_regs(register_list, module_name):

    rwmode_translate = {
        "rw"	: "RW",
        "rc"	: "RO",
        "rc"	: "RO",
        "r"	    : "RO",
        "w" 	: "RW",
        "rw"	: "RW",
        "e"     : "WO",
        "rc"	: "RO",
        "rwc"	: "RW",
    }

    size_translate = {
        'long'  : 4,
        'short' : 2,
        'char'  : 1,
    }

    row_template = ( "$\t%(register_name)-16s\t%(rwflags)2s\t"
            "0x%(offset)08X\t%(size)d\t%(window)d\t%(depth)s\n")
    regs = []
    for register in register_list:
        register_name = register['name']
        rwflags = rwmode_translate[register['rwmode']]
        offset = int(register['total_offset'])
        size = int(size_translate[register['wordsize']])
        window = 1
        depth = int(register['depth'])
        row = row_template % locals()
        regs.append(row)
    return ''.join(regs)

def gen_regs_file(register_list, module_name):
    """produce a .regs file"""

    name = module_name.upper()
    header = """NAME %s

  RegName		RwFlags	Offset		Size	Window	Depth

""" % name

    upper_name = name.upper()
    out = open(upper_name+'.regs', 'wb')
    out.write(header)
    out.write(gen_regs(register_list, upper_name))
    out.close()

def gen_makefile(lib_name, module_name, filename):
    """produce a Makefile for the library"""

    makefile_template = """
# generated automatically by encore, date %(date)s

LIBNAME = %(lib_name)s
REGFILE = %(module_name)s.regs

CPU=L865
include /acc/dsc/src/co/Make.auto

CFLAGS= -g -Wall -I. -I ../../driver
SRCS=$(LIBNAME).c $(LIBNAME).h

all: $(LIBNAME).$(CPU).a $(LIBNAME).$(CPU).so

$(LIBNAME).$(CPU).o: $(SRCS)

$(LIBNAME).$(CPU).a: $(LIBNAME).$(CPU).o
\t-$(RM) $@
\t$(AR) $(ARFLAGS) $@ $^
\t$(RANLIB) $@

$(LIBNAME).$(CPU).so: $(LIBNAME).$(CPU).o
\t-$(RM) $@
\t$(CC) $(CFLAGS) -o $@ -shared $^

clean:
\trm -f $(LIBNAME).$(CPU).o
\trm -f $(LIBNAME).$(CPU).a
\trm -f $(LIBNAME).$(CPU).so
"""

    date = datetime.datetime.now().isoformat(' ')
    makefile = makefile_template % locals()
    out = open(filename, 'wb')
    out.write(makefile)
    out.close()

def make_out_dir(dirname, kill=False):
    try:
        isdir = stat.S_ISDIR(os.stat(dirname).st_mode)
        if not kill and raw_input(dirname +
                ' already exists, remove? (y/N) ') != 'y':
            return -errno.ENOENT

        for root, dirs, files in os.walk(dirname, topdown=False):
            for name in files:
                os.remove(os.path.join(root, name))
            for name in dirs:
                os.rmdir(os.path.join(root, name))
        os.rmdir(dirname)
        return os.mkdir(dirname)
    except OSError:
        return os.mkdir(dirname)

def main():

    # option processing
    usage = 'usage: dbextract.py [options] MODULE_NAME'

    parser = OptionParser(usage=usage)
    parser.add_option("--plain", dest="plain", action="store",
                        help="create a plaintext file with register data",
                        metavar="FILE")
    parser.add_option("--csv", dest="csv", action="store",
                        help="create a CSV file with register data",
                        metavar="FILE")
    parser.add_option("--regs", action="store_true", dest="regs",
                        help="create a .regs file for encore v1.0 compat",
                        metavar="FILE")
    parser.add_option("-q", "--quiet",
                      action="store_false", dest="verbose", default=True,
                      help="don't print status messages to stdout")
    parser.add_option("-k", "--kill",
                      action="store_true", dest="kill", default=False,
                      help="kill destination directory if it exists")
    (options, args) = parser.parse_args(sys.argv)

    # only positional parameter, module name
    if len(args) != 2:
        parser.print_help()
        sys.exit(1)
    module_name = args[1].upper()

    # prepare output paths
    output_dir = module_name.lower()
    libname = 'lib%s' % module_name.lower()
    lib_filename = join(output_dir, libname + '.h')
    c_filename = join(output_dir, libname + '.c')
    make_filename = join(output_dir, 'Makefile')

    if make_out_dir(output_dir, options.kill):
        print 'could not make output dir ' + output_dir
        sys.exit(1)

    register_list = get_register_data(module_name)
    if not register_list:
        print 'no data for module %s in CCDB, exiting' % module_name
        os.rmdir(output_dir)
        sys.exit(1)

    # optional data files generation
    registers = dict([ (regdata['name'], regdata)
        for regdata in register_list ])
    if options.csv:
        csv_filename = join(output_dir, options.csv)
        gen_csv_file(register_list, csv_filename)
    if options.plain:
        plain_filename =join(output_dir, options.plain)
        gen_plain_file(register_list, plain_filename)
    if options.regs:
        regs_filename = join(output_dir, module_name)
        gen_regs_file(register_list, regs_filename)

    # library generation
    gen_h_file(register_list, libname, lib_filename)
    gen_c_file(register_list, libname, c_filename)
    gen_makefile(libname, module_name, make_filename)

if __name__ == '__main__':

    main()


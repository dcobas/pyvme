#!   /usr/bin/env	python
#    coding: utf8

from subprocess import Popen, PIPE
import csv
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
int %(driver_name)s_set_%(register_name)s (struct vsl_device *handle, %(argtype)s value);"""
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
int %(driver_name)s_set_%(register_name)s(struct vsl_device *handle, %(argtype)s value) {
    return usr_raw_write(%(offset_name)s, value);
}
"""
    calls = []
    for register in register_list:
        driver_name = prefix
        register_name = register['name']
        argtype = register['wordsize']
        offset_name = 'OFFSET_%s' % driver_name
        call = call_template % locals()
        calls.append(call)
    return ''.join(calls)

def gen_h_file(register_list, libname):
    """produce a lib<module>.h user library header"""
    h_file_header = """#include "vsl.h"\n\n"""

    out = open(libname+'.h', 'wb')
    out.write(h_file_header)
    out.write(gen_offsets(register_list))
    out.write('\n')
    out.write(gen_lib_decls(register_list))
    out.close()

def gen_c_file(register_list, libname):
    """produce a lib<module>.c user library module"""
    c_file_header = """#include "%s.h"\n\n""" % libname

    out = open(libname+'.c', 'wb')
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

def main():

    from pprint import pprint
    from sys import argv

    parser = OptionParser()
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

    (options, args) = parser.parse_args(argv)

    module_name = args[1]
    libname = 'lib%s' % module_name.lower()

    register_list = get_register_data(module_name)
    registers = dict([ (regdata['name'], regdata)
        for regdata in register_list ])
    if options.csv:
        gen_csv_file(register_list, options.csv)
    if options.plain:
        gen_plain_file(register_list, options.plain)
    if options.regs:
        gen_regs_file(register_list, module_name)

    # the lib is always generated
    gen_h_file(register_list, libname)
    gen_c_file(register_list, libname)

if __name__ == '__main__':

    main()


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

def make_plain_file(register_list, filename):
    """given a list of dicts with registers, construct a columnated file"""

    out = open(filename, 'wb')
    for reg in register_list:
        for key in field_list:
            out.write('%s\t' % reg[key])
        out.write('\n')
    out.close()

def make_csv_file(register_list, filename):
    """given a list of dicts with registers, construct a CSV file"""

    out = csv.DictWriter(open(filename, 'wb'), field_list)
    for reg in register_list:
        out.writerow(reg)

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
    parser.add_option("-q", "--quiet",
                      action="store_false", dest="verbose", default=True,
                      help="don't print status messages to stdout")

    (options, args) = parser.parse_args(argv)

    module_name = args[1]
    register_list = get_register_data(module_name)
    pprint(register_list)
    registers = dict([ (regdata['name'], regdata) 
        for regdata in register_list ])
    if options.csv:
        make_csv_file(register_list, options.csv)
    if options.plain:
        make_plain_file(register_list, options.plain)
    else:
        for reg in register_list:
            print reg

if __name__ == '__main__':

    main()

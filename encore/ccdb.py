#!   /usr/bin/env	python
#    coding: utf8

import csv
import cx_Oracle
import StringIO

db_am_translate = {
    2 : 0x39,
    4 : 0x2f,
    5 : 0x29,
    7 : 0x9,
}

register_field_list = [
    'name',
    'rwmode',
    'block',
    'block_address_space',
    'block_offset',
    'register_offset',
    'offset',
    'wordsize',
    'depth',
    'description',
    'data_width',
    ]

register_field_formats = [
    '%s', '%s', '%d', '%d', '0x%x', '0x%x', '0x%x', '%s', '%d', '%s',
    '%d',
]

register_query = '''
select name,
    rwmode,
    block,
    mb.address as block_address_space,
    nvl(mb.offsetval,0) as block_offsetval,
    mr.offsetval as register_offset,
    mr.offsetval + nvl(mb.offsetval,0) as offset,
    wordsize,
    nvl(depthval, 1) as depth,
    mr.description as description,
    case mb.address
    	when 1 then ht.initdsize
	when 2 then ht.nextdsize
    end as data_width
from moduleregisters mr join moduleblocks mb
        using (block, moduletype_id)
    join hard_types ht
        on moduletype_id = ht.hwtype_id
where ht.hwtype = :hwtype
order by block, block_offsetval, register_offset
'''

module_field_list = [
    'name',
    'description',
    'level',
    'data_width1',
    'data_width2',
    'size1',
    'size2',
    'am1',
    'am2'
]

module_field_formats = [
    '%s', '%s', '%d', '%d', '%d', '0x%x', '0x%x', '0x%x', '0x%x',
]

module_query = '''
select hwtype as name,
    description,
    ilevel,
    initdsize as data_width1,
    nextdsize as data_width2,
    initrangeval as size1,
    nextrangeval as size2,
    initasize as am1,
    nextasize as am2
from hard_types ht
where ht.hwtype = :hwtype
'''

def nullformat(fmt, val):
    if val is None:
         return ''
    else:
        return fmt % val

class CCDBTuple(dict):
    def __init__(self, *t):
        if len(t) == 1 and type(t[0]) is type(tuple()):
            self.update(zip(self.fields, t[0]))
        else:
            raise ValueError('CCDBTuple must be initialized with a tuple')

    def tuple(self):
        return tuple(self[field] for field in self.fields)

    def csv_tuple(self):
        return tuple(nullformat(fmt, self[field]) for (fmt, field)
            in zip(self.formats, self.fields))

class Register(CCDBTuple):
    fields  = register_field_list
    formats = register_field_formats

class Module(CCDBTuple):
    fields = module_field_list
    formats = module_field_formats

    def csv_dump(self, modfname, regfname, with_header=True):
        """convert module data to CSV files
        """
        out = file(modfname, 'w')
        c = csv.writer(out)
        if with_header:
            c.writerow(self.fields)
        c.writerow(self.csv_tuple())
        out.close()

        out = file(regfname, 'w')
        c = csv.writer(out)
        if with_header:
            c.writerow(Register.fields)
        c.writerows([r.csv_tuple() for r in self.registers])
        out.close()


def query_db(module_name):
    """get module data

    Return a module object with module data
    """

    cursor = cx_Oracle.connect(user='copub', password='co').cursor()
    cursor.execute(module_query, hwtype=module_name)
    modtuple = cursor.fetchone()
    module = Module(modtuple)

    cursor.execute(register_query, hwtype=module_name)
    module.registers = [ Register(row)  for row in cursor ]

    return module

def query_csv(module_data, register_data):
    sniff = csv.Sniffer()

    mod = StringIO.StringIO(module_data)
    if sniff.has_header(module_data):
        r = csv.DictReader(mod)
    else:
        r = csv.DictReader(mod, fieldnames=module_field_list)
    ret1 = r.next()

    reg = StringIO.StringIO(register_data)
    if sniff.has_header(register_data):
        r = csv.DictReader(reg)
    else:
        r = csv.DictReader(reg, fieldnames=register_field_list)
    ret2 = [ datum for datum in r ]
    
    return ret1, ret2

if __name__ == '__main__':
    import pprint
   
    if True:
        module_data = query_db('RF_VTU')
        module_data.csv_dump('rf_vtu_mod.csv', 'rf_vtu_reg.csv')
        pprint.pprint(query_csv(file('rf_vtu_mod.csv').read(), file('rf_vtu_reg.csv').read()))


    if False:
        pprint.pprint(query_csv(file('ctc_mod.csv').read(), file('ctc_reg.csv').read()))

    if False:
        x = Register( ('Control_1','rw',0,1,0,0,0,'short',0,"Control 1, 8 bit Read-Modify-Write",16) )
        print x
        print x.tuple()
        print x.csv_tuple()

#!   /usr/bin/env	python
#    coding: utf8

import csv
import cx_Oracle
import StringIO

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

class NoModule(Exception):
    pass

def nullformat(fmt, val):
    if val is None or val is '':
         return ''
    else:
        return fmt % val

class CCDBTuple(dict):
    def __init__(self, *t):
        if len(t) != 1:
            raise ValueError('CCDBTuple must be initialized with a tuple')
        if not type(t[0]) in [ type(tuple()), type(list()) ]:
            raise ValueError('CCDBTuple must be initialized with a tuple')

        # constructor called with a single tuple
        self.update(zip(self.fields, t[0]))
        for i in range(len(self.fields)):
            if self[self.fields[i]] is None:
                self[self.fields[i]] = ''
                continue
            if not self[self.fields[i]]:
                continue
            if (self.formats[i][-1] in 'dx' and
                    type(self[self.fields[i]]) == type('')):
                # coerce string into int
                self[self.fields[i]] = int(self[self.fields[i]], 0)

    def tuple(self):
        return tuple(self[field] for field in self.fields)

    def csv_tuple(self):
        return tuple(nullformat(fmt, self[field]) for (fmt, field)
            in zip(self.formats, self.fields))

class Register(CCDBTuple):
    fields  = register_field_list
    formats = register_field_formats

    def cdecl(self):
        return (
            '[%(name)s] = {\n'
            '	.name			= "%(name)s",\n'
            '	.rwmode			= "%(rwmode)s",\n'
            '	.block			= %(block)d,\n'
            '	.block_address_space	= %(block_address_space)d,\n'
            '	.block_offset		= 0x%(block_offset)x,\n'
            '	.register_offset	= 0x%(register_offset)x,\n'
            '	.offset			= 0x%(offset)x,\n'
            '	.wordsize		= "%(wordsize)s",\n'
            '	.depth			= 0x%(depth)x,\n'
            '	.description		= "%(description)s",\n'
            '	.data_width		= %(data_width)d,\n'
            '},\n'
            '\n'
            ) % self

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
    if not modtuple:
        raise NoModule('%s not declared in CCDB' % module_name)
    module = Module(modtuple)

    cursor.execute(register_query, hwtype=module_name)
    module.registers = [ Register(row)  for row in cursor ]

    return module

def query_csv(modfname, regfname):
    sniff = csv.Sniffer()

    mod = file(modfname)
    if sniff.has_header(mod.read()):
        mod.seek(0)
        mod.readline()
    else:
        mod.seek(0)
    r = csv.reader(mod)
    module = Module(r.next())

    reg = file(regfname)
    if sniff.has_header(reg.read()):
        reg.seek(0)
        reg.readline()
    else:
        reg.seek(0)
    r = csv.reader(reg)
    module.registers = [ Register(reg) for reg in r ]
    
    return module

if __name__ == '__main__':
    import pprint
   
    if False:
        module_data = query_db('RF_VTU')
        module_data.csv_dump('rf_vtu_mod.csv', 'rf_vtu_reg.csv')
        pprint.pprint(query_csv('rf_vtu_mod.csv', 'rf_vtu_reg.csv'))
        pprint.pprint(query_csv('rf_vtu_mod.csv', 'rf_vtu_reg.csv').registers)

    if True:
        pprint.pprint(query_csv('ctc_mod.csv', 'ctc_reg.csv'))
        pprint.pprint(query_csv('ctc_mod.csv', 'ctc_reg.csv').registers)

    if False:
        x = Register( ('Control_1','rw',0,1,0,0,0,'short',0,"Control 1, 8 bit Read-Modify-Write",16) )
        print x
        print x.tuple()
        print x.csv_tuple()

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

cursor = cx_Oracle.connect(user='copub', password='co').cursor()

def query_db(module_name):
    """get module data

    Return a dictionary with module data, and a list of dicts
    with register properties
    """

    cursor.execute(module_query, hwtype=module_name)
    modtuple = cursor.fetchone()
    module_data = dict(zip(module_field_list, modtuple))

    cursor.execute(register_query, hwtype=module_name)
    register_data = [ dict(zip(register_field_list, row)) for row in cursor ]

    return module_data, register_data

def csv_module_data(module_data, with_header=True):
    """convert module_data dict to CSV row

    Return a string with the CSV form of the data
    """
    out = StringIO.StringIO()
    c = csv.DictWriter(out, fieldnames=module_field_list)
    if with_header:
        c.writerow(dict(zip(module_field_list, module_field_list)))
    c.writerow(module_data)
    ret = out.getvalue()
    out.close()
    return ret 

def csv_register_data(register_data, with_header=True):
    """convert register data of a module into CSV form

    Returns a string with the CSV form of the data
    """
    out = StringIO.StringIO()
    c = csv.DictWriter(out, fieldnames=register_field_list)
    if with_header:
        c.writerow(dict(zip(register_field_list, register_field_list)))
    c.writerows(register_data)
    ret = out.getvalue()
    out.close()
    return ret 

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
   
    if False:
        module_data, register_data = query_db('RF_VTU')
        file('rf_vtu_mod.csv', 'w').write(csv_module_data(module_data))
        file('rf_vtu_reg.csv', 'w').write(csv_register_data(register_data))
        print query_csv(file('rf_vtu_mod.csv').read(), file('rf_vtu_reg.csv').read())


    pprint.pprint(query_csv(file('ctc_mod.csv').read(), file('ctc_reg.csv').read()))

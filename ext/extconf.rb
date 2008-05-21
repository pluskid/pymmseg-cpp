require 'mkmf'

CONFIG['LDSHARED'] = 'g++ -shared'

$objs = ['complex.o', 'dict.o', 'memory.o', 'rmmseg.o']
create_makefile('rmmseg')

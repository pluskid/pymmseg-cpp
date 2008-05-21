require 'mkmf'

CONFIG['LDSHARED'] = 'g++ -shared'

$objs = ['algor.o', 'dict.o', 'memory.o', 'rmmseg.o']
create_makefile('rmmseg')

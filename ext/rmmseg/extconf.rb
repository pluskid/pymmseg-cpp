require 'mkmf'

CONFIG['LDSHARED'] = CONFIG['LDSHARED'].sub(/^\$\(CC\)/, 'g++')

# if RUBY_PLATFORM =~ /darwin/
# #  CONFIG['LDSHARED'] = 'g++ --dynamiclib -flat_namespace -undefined suppress' 
#   CONFIG['LDSHARED'] = 'g++ --dynamiclib'
# elsif RUBY_PLATFORM =~ /linux/
#   CONFIG['LDSHARED'] = 'g++ -shared'  
# end

$objs = ['algor.o', 'dict.o', 'memory.o', 'rmmseg.o']
create_makefile('rmmseg')

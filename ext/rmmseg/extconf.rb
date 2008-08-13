require 'mkmf'

CONFIG['LDSHARED'] = CONFIG['LDSHARED'].sub(/^\$\(CC\)/, 'g++')

# if RUBY_PLATFORM =~ /darwin/
# #  CONFIG['LDSHARED'] = 'g++ --dynamiclib -flat_namespace -undefined suppress' 
#   CONFIG['LDSHARED'] = 'g++ --dynamiclib'
# elsif RUBY_PLATFORM =~ /linux/
#   CONFIG['LDSHARED'] = 'g++ -shared'  
# end

if RUBY_PLATFORM =~ /darwin/
  CONFIG['LDShARED'] = 'g++ -dynamiclib -single_module -flat_namespace -undefined suppress'
end

$objs = ['algor.o', 'dict.o', 'memory.o', 'rmmseg.o']
create_makefile('rmmseg')

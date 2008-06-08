require 'mkmf'

if RUBY_PLATFORM =~ /darwin/
  CONFIG['LDSHARED'] = 'g++ --dynamiclib -flat_namespace -undefined suppress' 
elsif RUBY_PLATFORM =~ /linux/
  CONFIG['LDSHARED'] = 'g++ -shared'  
end

$objs = ['algor.o', 'dict.o', 'memory.o', 'rmmseg.o']
create_makefile('rmmseg')

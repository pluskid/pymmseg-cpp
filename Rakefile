# Look in the tasks/setup.rb file for the various options that can be
# configured in this Rakefile. The .rake files in the tasks directory
# are where the options are used.

load 'tasks/setup.rb'

ensure_in_path 'lib'
require 'rmmseg'

task :default => 'spec:run'

PROJ.name = 'rmmseg-cpp'
PROJ.version = '0.2.4'
PROJ.authors = 'pluskid'
PROJ.email = 'pluskid@gmail.com'
PROJ.url = 'http://rmmseg-cpp.rubyforge.org'
PROJ.rubyforge.name = 'rmmseg-cpp'

PROJ.spec.opts << '--color'

# EOF

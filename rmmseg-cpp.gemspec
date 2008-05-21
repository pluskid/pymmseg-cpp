# -*- Ruby -*-
spec = Gem::Specification.new do |s|
  s.platform         =   Gem::Platform::RUBY
  s.name             =   "rmmseg-cpp"
  s.version          =   "0.2.1"
  s.author           =   "pluskid"
  s.email            =   "pluskid@gmail.com"
  s.summary          =   "A high performance package for doing Chinese word segmentation."
  s.files            =   Dir.glob('lib/**/*.rb')  +
                             Dir.glob('ext/rmmseg/*.{cpp,h}') +
                             Dir.glob('data/*')               +
                             ['README', 'ext/rmmseg/extconf.rb']
  s.require_path     =   'lib'
  s.extensions      <<   'ext/rmmseg/extconf.rb'
  s.has_rdoc         =   true
  s.rdoc_options    <<   '--inline-source'    <<
                         '--main' << 'README' <<
                         '--line-numbers'
  s.extra_rdoc_files =   ['README']
end

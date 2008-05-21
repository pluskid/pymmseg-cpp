# -*- Ruby -*-
spec = Gem::Specification.new do |s|
  s.platform = Gem::Platform::RUBY
  s.name = "rmmseg-cpp"
  s.version = "0.2.0"
  s.author = "pluskid"
  s.email = "pluskid@gmail.com"
  s.summary = "A high performance package for doing Chinese word segmentation."
  s.files = Dir.glob('lib/**/*.rb')  +
    Dir.glob('ext/rmmseg/*.{cpp,h}') +
    Dir.glob('data/*')               +
    ['README', 'ext/rmmseg/extconf.rb']
  s.require_paths = ['lib', 'ext']
  s.extensions << 'ext/rmmseg/extconf.rb'
end

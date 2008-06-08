rmmseg-cpp
    by pluskid
    http://rmmseg-cpp.rubyforge.org

== DESCRIPTION:

rmmseg-cpp is a re-written of RMMSeg in C++. The interface is almost
identical to RMMSeg but the performance is much better. This gem
is always preferable in production use.

RMMSeg (http://rmmseg.rubyforge.org) is a Chinese word segmentation library
written for and in Ruby. It features full integration with Ruby. However, 
its performance (both time and memory) is terrible in some cases, especially
when you use the complex algorithm.

== FEATURES/PROBLEMS:

* Runs fast and the memory consumption is small.
* Support user customized dictionaries.
* Easy Ferret integration.

== SYNOPSIS:

=== A simple script

rmmseg-cpp provides a simple script, which can read the text from
standard input and print the segmented result to standard output. Try
<tt>rmmseg -h</tt> for help on the options.

=== In a normal Ruby program

To use rmmseg-cpp in normal Ruby program, first load the package and
init by loading the dictionaries:

  require 'rubygems'
  require 'rmmseg'

  RMMSeg::Dictionary.load_dictionaries

Then create a +Algorithm+ object and call +next_token+ until got a
+nil+:

  algor = RMMSeg::Algorithm.new(text)
  loop do
    tok = algor.next_token
    break if tok.nil?
    puts "#{tok.text} [#{tok.start}..#{tok.end}]"
  end

=== With Ferret

To use rmmseg-cpp with Ferret, just use the analyzer provided:

  analyzer = RMMSeg::Ferret::Analyzer.new { |tokenizer|
    Ferret::Analysis::LowerCaseFilter.new(tokenizer)
  }

  index = Ferret::Index::Index.new(:analyzer => analyzer)

See <tt>misc/ferret_example.rb</tt> for a complete example.

== REQUIREMENTS:

* ruby 1.8.x
* g++

== INSTALL:

* sudo gem install rmmseg-cpp

== LICENSE:

(The MIT License)

Copyright (c) 2008 FIXME (different license?)

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
'Software'), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

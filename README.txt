rmmseg-cpp
    by pluskid
    http://rmmseg-cpp.rubyforge.org

== DESCRIPTION:

rmmseg-cpp is a re-written of RMMSeg in C++. The interface is almost
identical to RMMSeg but the performance is much better. This package
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

FIXME

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

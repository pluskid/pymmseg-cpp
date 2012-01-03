#!/usr/bin/env python
import sys
from distutils.core import setup, Extension
from distutils.command.build_ext import build_ext as _build_ext


class build_ext(_build_ext):
    def finalize_options(self):
        _build_ext.finalize_options(self)
        self.build_temp = './'
        self.build_lib = '../'

if len(sys.argv) == 1:
    sys.argv.append('build')

setup(ext_modules=[
    Extension(
        '_mmseg',
        sources=['mmseg.cpp', 'algor.cpp', 'dict.cpp', 'memory.cpp']
    ),
], cmdclass={'build_ext': build_ext})

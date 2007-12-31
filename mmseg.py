# -*- coding: utf-8 -*-

import sys
from ctypes import *
from os.path import join, dirname, abspath, exists

if sys.platform == 'win32':
    ext = 'dll'
else:
    ext = 'so'
    
mmseg = cdll.LoadLibrary(join(dirname(__file__),
                              'mmseg-cpp',
                              'mmseg.%s' % ext))

########################################
# the Token struct
########################################
class Token(Structure):
    _fields_ = [('_text', c_void_p),
                ('_offset', c_int),
                ('_length', c_int)]

    def text_get(self):
        return string_at(self._text, self._length)
    def text_set(self, value):
        raise AttributeError('text attribute is read only')
    text = property(text_get, text_set)

    def start_get(self):
        return self._offset
    def start_set(self, value):
        raise AttributeError('start attribute is read only')
    start = property(start_get, start_set)

    def end_get(self):
        return self._offset+self._length
    def end_set(self, value):
        raise AttributeError('start attribute is read only')
    end = property(end_get, end_set)

    def length_get(self):
        return self._length
    def length_set(self):
        raise AttributeError('length attribute is read only')
    length = property(length_get, length_set)

    def __repr__(self):
        return '<Token %d..%d %s>' % (self.start,
                self.end, self.text.__repr__())
    def __str__(self):
        return self.text


########################################
# Init function prototypes
########################################
mmseg.mmseg_load_chars.argtypes = [c_char_p]
mmseg.mmseg_load_chars.restype  = c_int

mmseg.mmseg_load_words.argtypes = [c_char_p]
mmseg.mmseg_load_words.restype  = c_int

mmseg.mmseg_dic_add.argtypes = [c_char_p, c_int, c_int]
mmseg.mmseg_dic_add.restype  = None

mmseg.mmseg_algor_create.argtypes = [c_char_p, c_int]
mmseg.mmseg_algor_create.restype  = c_void_p

mmseg.mmseg_algor_destroy.argtypes = [c_void_p]
mmseg.mmseg_algor_destroy.restype  = None

mmseg.mmseg_next_token.argtypes = [c_void_p]
mmseg.mmseg_next_token.restype  = Token


########################################
# Python API
########################################
def dict_load_chars(path):
    res = mmseg.mmseg_load_chars(path)
    if res == 0:
        return False
    return True

def dict_load_words(path):
    res = mmseg.mmseg_load_words(path)
    if res == 0:
        return False
    return True

def dict_load_defaults():
    mmseg.mmseg_load_chars(join(dirname(__file__), 'data', 'chars.dic'))
    mmseg.mmseg_load_words(join(dirname(__file__), 'data', 'words.dic'))

class Algorithm(object):
    def __init__(self, text):
        """\
        Create an Algorithm instance to segment text.
        """
        self.text      = text # add a reference to prevent the string buffer from 
                              # being GC-ed
        self.algor     = mmseg.mmseg_algor_create(text, len(text))
        self.destroied = False

    def __iter__(self):
        """\
        Iterate through all tokens. Note the iteration has
        side-effect: an Algorithm object can only be iterated
        once.
        """
        while True:
            tk = self.next_token()
            if tk is None:
                raise StopIteration
            yield tk
    
    def next_token(self):
        """\
        Get next token. When no token available, return None.
        """
        if self.destroied:
            return None
        
        tk = mmseg.mmseg_next_token(self.algor)
        if tk.length == 0:
            # no token available, the algorithm object
            # can be destroied
            self._destroy()
            return None
        else:
            return tk

    def _destroy(self):
        
        if not self.destroied:
            mmseg.mmseg_algor_destroy(self.algor)
            self.destroied = True

    def __del__(self):
        self._destroy()

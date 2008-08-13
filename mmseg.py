# -*- coding: utf-8 -*-

from ctypes import *
from os.path import join, dirname, abspath, exists

mmseg = cdll.LoadLibrary(join(dirname(__file__),
                              'mmseg-cpp',
                              'mmseg.so'))

########################################
# the rmmseg::Token struct
########################################
class Token(Structure):
    _fields_ = [('text', c_void_p),
                ('length', c_int)]


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


mmseg.mmseg_load_chars(join(dirname(__file__), 'data', 'chars.dic'))
mmseg.mmseg_load_words(join(dirname(__file__), 'data', 'words.dic'))

text = "这不是一段中文文本"
algor = mmseg.mmseg_algor_create(text, len(text))

while True:
    tok = mmseg.mmseg_next_token(algor)
    if tok.length == 0:
        break
    print string_at(tok.text, tok.length)

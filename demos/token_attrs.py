#!/usr/bin/env python
#-*- coding:utf-8 -*-
import os
import mmseg


PWD = os.path.dirname(os.path.realpath(__file__))
my_dict_path = os.path.join(PWD, 'my-words.dic')


def test_token_attrs():
    new_dicts = list(mmseg.Dictionary.dictionaries)
    new_dicts.append(('words', my_dict_path))
    mmseg.Dictionary.dictionaries = new_dicts

    mmseg.Dictionary.load_dictionaries()

    text = u'人妻女教师'
    for token in mmseg.Algorithm(text):
        print '%s (%d %d..%d)' % (token.text, token.start, token.end, token.length)

if __name__ == '__main__':
    test_token_attrs()
#!/usr/bin/env python
#-*- coding:utf-8 -*-
import os
import mmseg


PWD = os.path.dirname(os.path.realpath(__file__))
my_dict_path = os.path.join(PWD, 'my-words.dic')

def get_words():
    with open(my_dict_path) as f:
        words = set()
        for line in f.readlines():
            length, word = line.split()
            words.add(word.strip())
        return words

def test_use_custom_dict():
    new_dicts = list(mmseg.Dictionary.dictionaries)
    new_dicts.append(('words', my_dict_path))
    mmseg.Dictionary.dictionaries = new_dicts

    mmseg.Dictionary.load_dictionaries()

    words = get_words()
    
    texts = (
        u'人妻女教师',
        u'齐逼小短裤',
        u'齐b小短裤',
    )

    for text in texts:
        for token in mmseg.Algorithm(text):
            # print '%s [%d..%d]' % (token.text, token.start, token.end)
            assert token.text in words
            # try:
            #     assert token.text in words
            # except AssertionError:
            #     print type(token.text), token.text
    
  
if __name__ == '__main__':
    test_use_custom_dict()
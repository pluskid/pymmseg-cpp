#!/usr/bin/env python
#-*- coding:utf-8 -*-
"""
generate a custom dictionary file from a plain text file

    python plain2dic.py my-words.txt | sort | uniq > my-words.dic

test it

    python use_custom_dict.py
"""
import os
import mmseg


PWD = os.path.dirname(os.path.realpath(__file__))
my_dict_path = os.path.join(PWD, 'my-words.dic')


def test_use_custom_dict():
    mmseg.Dictionary.load_dictionaries()

    texts = (
        u'他来到了网易杭研大厦',
        u'李小福是创新办主任也是云计算方面的专家',
        u'工信处女干事每月经过下属科室都要亲口交代24口交换机等技术性器件的安装工作',
    )

    for text in texts:
        for token in mmseg.Algorithm(text):
            print '%s' % (token.text),
        print

    new_dicts = list(mmseg.Dictionary.dictionaries)
    new_dicts.append(('words', my_dict_path))
    mmseg.Dictionary.dictionaries = new_dicts
    mmseg.Dictionary.load_dictionaries()

    print
    for text in texts:
        for token in mmseg.Algorithm(text):
            print '%s' % (token.text),
        print
    
  
if __name__ == '__main__':
    test_use_custom_dict()
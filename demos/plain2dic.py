#!/usr/bin/env python
import sys

import langconv

conv_handler = langconv.ConverterHandler('zh-cn')
parse_rules = True

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print 'Usage: %s <file1> [file2 ...]' % sys.argv[0]
        exit(0)

    for file_path in sys.argv[1:]:
        with open(file_path) as f:
            lines = f.readlines()
            for line in lines:

                word = line.strip()
                if not word:
                    continue

                word = conv_handler.convert(word.decode('utf-8'), parse_rules)
                print '%d %s' % (len(word), word.encode('utf-8'))


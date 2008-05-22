#!/usr/bin/ruby

# A utility used to convert the old RMMSeg dictionary
# to rmmseg-cpp format.

# There are several constrains for the new rmmseg-cpp
# dictionary format.
#  - length of word should be specified in the dict
#  - number and string should be separated by ONE space
#  - there should be a newline at the end of file

$KCODE='u'
require 'jcode'

def usage(msg=nil)
  puts "***ERROR: #{msg}\n\n" if msg
  puts <<EOT
Usage:

#{$0} action type input.dic output.dic

  action: either 'convert' or 'normalize'
           - 'convert' is used to convert the dict from
             old RMMSeg format.
           - 'normalize' is used to normalize an existing
             rmmseg-cpp dict.

  type:   either 'words' or 'chars'

EOT
  exit(0)
end

usage if ARGV.size != 4
usage("unknown action #{ARGV[0]}") if ! ['convert', 'normalize'].include? ARGV[0]
usage("unknown type #{ARGV[1]}") if ! ['words', 'chars'].include? ARGV[1]

def output(data)
  File.open(ARGV[3], "w") do |f|
    data.each do |num, word|
      f.puts "#{num} #{word}" if word
    end
  end
end

def read_RMMSeg_chars
  max = 0
  File.readlines(ARGV[2]).map do |line|
    if line =~ /^(.)\s+(\d+)$/
      n = $2.to_i
      max = n if n > max
      [n, $1]
    else
      [nil, nil]
    end
  end.map do |num, word|
    if word
      [num*65535/max, word]
    else
      [nil, nil]
    end
  end
end

def read_RMMSeg_words
  File.readlines(ARGV[2]).map do |line|
    line.chomp!
    if !line.empty?
      [line.jlength, line]
    else
      [nil, nil]
    end
  end
end

def read_rmmseg_cpp_chars
  max = 0
  File.readlines(ARGV[2]).map do |line|
    if line =~ /^(\d+)\s+(.)$/
      n = $1.to_i
      max = n if n > max
      [n, $2]
    else
      [nil, nil]
    end
  end.map do |num, word|
    if word
      [num*65535/max, word]
    else
      [nil, nil]
    end
  end
end

def read_rmmseg_cpp_words
  File.readlines(ARGV[2]).map do |line|
    if line =~ /^(\d+)\s+(\w+)$/
      [$1, $2]
    else
      [nil, nil]
    end
  end
end

case ARGV[0,2]
when ['convert', 'chars']
  output(read_RMMSeg_chars)
when ['convert', 'words']
  output(read_RMMSeg_words)
when ['normalize', 'chars']
  output(read_rmmseg_cpp_chars)
when ['normalize', 'words']
  output(read_rmmseg_cpp_words)
end

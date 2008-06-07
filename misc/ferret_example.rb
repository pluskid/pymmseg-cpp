#!/usr/bin/env ruby
require 'rubygems'
require 'rmmseg'
require 'rmmseg/ferret'

# dictionaries needed to be explicitly loaded
RMMSeg::Dictionary.load_dictionaries
  
analyzer = RMMSeg::Ferret::Analyzer.new { |tokenizer|
  Ferret::Analysis::LowerCaseFilter.new(tokenizer)
}

$index = Ferret::Index::Index.new(:analyzer => analyzer)

$index << {
  :title => "分词",
  :content => "中文分词比较困难，不像英文那样，直接在空格和标点符号的地方断开就可以了。"
}
$index << {
  :title => "RMMSeg",
  :content => "RMMSeg 我近日做的一个 Ruby 中文分词实现，下一步是和 Ferret 进行集成。"
}
$index << {
  :title => "Ruby 1.9",
  :content => "Ruby 1.9.0 已经发布了，1.9 的一个重大改进就是对 Unicode 的支持。"
}
$index << {
  :title => "Ferret",
  :content => <<END
Ferret is a high-performance, full-featured text search engine library
written for Ruby. It is inspired by Apache Lucene Java project. With
the introduction of Ferret, Ruby users now have one of the fastest and
most flexible search libraries available. And it is surprisingly easy
to use.
END
}

def highlight_search(key)
  $index.search_each(%Q!content:"#{key}"!) do |id, score|
    puts "*** Document \"#{$index[id][:title]}\" found with a score of #{score}"
    puts "-"*40
    highlights = $index.highlight("content:#{key}", id,
                                  :field => :content,
                                  :pre_tag => "\033[36m",
                                  :post_tag => "\033[m")
    puts "#{highlights}"
    puts ""
  end
end

ARGV.each { |key|
  puts "\033[33mSearching for #{key}...\033[m"
  puts ""
  highlight_search(key)
}

# Local Variables:
# coding: utf-8
# End:

require 'rubygems'
require 'rmmseg'
require 'ferret'

module RMMSeg
  module Ferret
        # The Analyzer class can be used with Ferret .
    class Analyzer < ::Ferret::Analysis::Analyzer
      
      # Construct an Analyzer. Optional block can be used to
      # add more +TokenFilter+s. e.g.
      #
      #   analyzer = RMMSeg::Ferret::Analyzer.new { |tokenizer|
      #     Ferret::Analysis::LowerCaseFilter.new(tokenizer)
      #   }
      #
      def initialize(&brk)
        @brk = brk
      end
      
      def token_stream(field, text)
        t = Tokenizer.new(text)
        if @brk
          @brk.call(t)
        else
          t
        end
      end
    end

    # The Tokenizer tokenize text with RMMSeg::Algorithm.
    class Tokenizer < ::Ferret::Analysis::TokenStream
      # Create a new Tokenizer to tokenize +text+
      def initialize(str)
        self.text = str
      end

      # Get next token
      def next
        tok = @algor.next_token
        if tok.nil?
          return nil
        else
          @token.text = tok.text
          @token.start = tok.start
          @token.end = tok.end
          return @token
        end
      end
      
      # Get the text being tokenized
      def text
        @text
      end

      # Set the text to be tokenized
      def text=(str)
        @token = ::Ferret::Analysis::Token.new("", 0, 0)
        @text = str
        @algor = Algorithm.new(@text)
      end
    end
  end
end

module RMMSeg
  module Dictionary
    @dictionaries = [
                     [:chars, File.join(File.dirname(__FILE__),
                                        "..", "..", "data", "chars.dic")],
                     [:words, File.join(File.dirname(__FILE__),
                                        "..", "..", "data", "words.dic")]
                    ]

    class << self
      #
      # An array of dictionaries used by RMMSeg. Each entry is of the
      # following form:
      #
      #   [type, path]
      #
      # where +type+ can either <tt>:chars</tt> or <tt>:words</tt>. +path+ is the path
      # to the dictionary file.
      #
      # The format of <tt>:chars</tt> dictionary is a collection of lines of the
      # following form:
      #
      #   freq char
      #
      # Where +frequency+ is a number <b>less than 65535</b>. +char+ is the
      # character. They are spearated by <b>exactly one space</b>.
      #
      # The format of <tt>:words</tt> dictionary is similar:
      #
      #   length word
      #
      # except the first number is not the frequency, but the number of
      # characters (not number of bytes) in the word.
      #
      # There's a script (convert.rb) in the tools directory that can be used
      # to convert and normalize dictionaries.
      attr_accessor :dictionaries

      # Add a user defined dictionary, +type+ can be
      # +:chars+ or <tt>:words</tt>. See doc of dictionaries.
      def add_dictionary(path, type)
        @dictionaries << [type, path]
      end

      # Load dictionaries. Call this method after set up the path of the
      # dictionaries needed to load and before any Algorithm object is
      # created.
      def load_dictionaries()
        @dictionaries.each do |type, path|
          if type == :chars
            load_chars(path)
          elsif type == :words
            load_words(path)
          end
        end
      end
    end
  end
end

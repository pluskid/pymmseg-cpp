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
      # where +type+ can either +:chars+ or +:words+. +path+ is the path
      # to the dictionary file.
      #
      # The format of +:chars+ dictionary is a collection of lines of the
      # following form:
      #
      #   freq char
      #
      # Where +frequency+ is a number *less than 65535*. +char+ is the
      # character. They are spearated by *exactly one space*.
      #
      # The format of +:words+ dictionary is similar:
      #
      #   length word
      #
      # except the first number is not the frequency, but the number of
      # characters (not number of bytes) in the word.
      # 
      attr_accessor :dictionaries

      # Add a user defined dictionary, +type+ can be
      # +:chars+ or +:words+. See doc of dictionaries.
      def add_dictionary(path, type)
        @dictionaries << [type, path]
      end

      def load_dictionaries
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

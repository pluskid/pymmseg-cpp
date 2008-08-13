/**
 * A C interface to mmseg-cpp. Will be compiled to a
 * shared library.
 */

#include "token.h"
#include "dict.h"
#include "algor.h"

extern "C" {
    /*
     * Load a character dictionary.
     */ 
    int mmseg_load_chars(const char *path)
    {
        if (rmmseg::dict::load_chars(path))
            return 1;
        return 0;
    }

    /*
     * Load a word dictionary.
     */ 
    int mmseg_load_words(const char *path)
    {
        if (rmmseg::dict::load_words(path))
            return 1;
        return 0;
    }
    
    /*
     * Add a word to the in-memory dictionary.
     *
     * - word is a String.
     * - length is number of characters (not number of bytes) of the
     *   word to be added.
     * - freq is the frequency of the word. This is only used when
     *   it is a one-character word.
     */
    void mmseg_dic_add(const char *word, int len, int freq)
    {
        rmmseg::Word *w = rmmseg::make_word(word, len, freq, strlen(word));
        rmmseg::dict::add(w);
    }
}

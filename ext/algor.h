#ifndef _ALGORITHM_H_
#define _ALGORITHM_H_

#include <vector>

#include "chunk.h"
#include "token.h"
#include "dict.h"

/**
 * The Algorithm of MMSeg use four rules:
 *  - Maximum matching rule
 *  - Largest average word length rule
 *  - Smallest variance of word length rule
 *  - Largest sum of degree of morphemic freedom of one-character
 *    words rule
 */

namespace rmmseg
{
    class Algorithm
    {
    public:
        Algorithm(const char *text, int length)
            :m_text(text), m_pos(0),
            m_text_length(length) { }

        Token next_token();

    private:
        Token get_basic_latin_word();
        Token get_cjk_word(int);
        
        std::vector<Chunk> create_chunks();
        int next_word();
        int next_char();
        std::vector<Word *> find_match_words();
        int max_word_length() { return 4; }

        
        const char *m_text;
        int m_pos;
        int m_text_length;

        /* tmp words are only for 1-char words which
         * are not exist in the dictionary. It's length
         * value will be set to -1 to indicate it is
         * a tmp word. */
        Word *get_tmp_word();
        std::vector<Word> m_tmp_words;
        int m_tmp_words_i;
    };
}

#endif /* _ALGORITHM_H_ */

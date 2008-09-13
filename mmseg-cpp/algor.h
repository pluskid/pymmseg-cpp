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
            m_text_length(length),
            m_tmp_words_i(0),
            m_match_cache_i(0)
            {
                for (int i = 0; i < match_cache_size; ++i)
                    m_match_cache[i].first = -1;
            }

        Token next_token();

        const char *get_text() const
        {
            return m_text;
        }

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
        Word *get_tmp_word()
        {
            if (m_tmp_words_i >= max_tmp_words)
                m_tmp_words_i = 0;  // round wrap
            return &m_tmp_words[m_tmp_words_i++];
        }

        /* related to max_word_length and match_words_cache_size */
        static const int max_tmp_words = 64;
        Word m_tmp_words[max_tmp_words];
        int m_tmp_words_i;

        /* match word caches */
        static const int match_cache_size = 3;
        typedef std::pair<int, std::vector<Word *> > match_cache_t;
        match_cache_t m_match_cache[match_cache_size];
        int m_match_cache_i;
    };
}

#endif /* _ALGORITHM_H_ */

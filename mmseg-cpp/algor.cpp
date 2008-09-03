#include <cctype>
#include <cassert>

#include "rules.h"
#include "algor.h"

using namespace std;

namespace rmmseg
{
    Token Algorithm::next_token()
    {
        do
        {
            if (m_pos >= m_text_length)
                return Token(NULL, 0);
     
            Token tk(NULL, 0);
            int len = next_char();
            if (len == 1)
                tk = get_basic_latin_word();
            else
                tk = get_cjk_word(len);
            if (tk.length > 0)
                return tk;
        }
        while (true);
    }

    Token Algorithm::get_basic_latin_word()
    {
        int len = 1;
        int start, end;

        // Skip pre-word whitespaces and punctuations
        while (m_pos < m_text_length)
        {
            if (len > 1)
                break;
            if (isalnum(m_text[m_pos]))
                break;
            m_pos++;
            len = next_char();
        }

        start = m_pos;
        while (m_pos < m_text_length)
        {
            if (len > 1)
                break;
            if (!isalnum(m_text[m_pos]))
                break;
            m_pos++;
            len = next_char();
        }
        end = m_pos;

        // Skip post-word whitespaces and punctuations
        while (m_pos < m_text_length)
        {
            if (len > 1)
                break;
            if (isalnum(m_text[m_pos]))
                break;
            m_pos++;
            len = next_char();
        }

        return Token(m_text+start, end-start);
    }

    Token Algorithm::get_cjk_word(int len)
    {
        create_chunks();

        if (m_chunks_size > 1)
            m_chunks_size = mm_filter(m_chunks, m_chunks_size);
        if (m_chunks_size > 1)
            m_chunks_size = lawl_filter(m_chunks, m_chunks_size);
        if (m_chunks_size > 1)
            m_chunks_size = svwl_filter(m_chunks, m_chunks_size);
        if (m_chunks_size > 1)
            m_chunks_size = lsdmfocw_filter(m_chunks, m_chunks_size);

        if (m_chunks_size < 1)
            return Token(NULL, 0);

        Token token(m_text+m_pos, m_chunks[0].words[0]->nbytes);
        m_pos += m_chunks[0].words[0]->nbytes;
        return token;
    }
    
    void Algorithm::create_chunks()
    {
        Chunk chunk;
        Word *w1, *w2, *w3;

        int orig_pos = m_pos;
        typedef vector<Word *> vec_t;
        typedef vec_t::iterator it_t;

        vec_t words1 = find_match_words();
        m_chunks_size = 0;

        for (it_t i1 = words1.begin();
             i1 != words1.end();
             ++i1)
        {
            w1 = *i1;
            chunk.words[0] = w1;
            m_pos += w1->nbytes;
            if (m_pos < m_text_length)
            {
                vec_t words2 = find_match_words();
                for (it_t i2 = words2.begin();
                     i2 != words2.end();
                     ++i2)
                {
                    w2 = *i2;
                    chunk.words[1] = w2;
                    m_pos += w2->nbytes;
                    if (m_pos < m_text_length)
                    {
                        vec_t words3 = find_match_words();
                        for (it_t i3 = words3.begin();
                             i3 != words3.end();
                             ++i3)
                        {
                            w3 = *i3;
                            if (w3->length == -1) // tmp word
                            {
                                chunk.n = 2;
                            }
                            else
                            {
                                chunk.n = 3;
                                chunk.words[2] = w3;
                            }
                            memcpy(m_chunks+m_chunks_size, &chunk,
                                   sizeof(Chunk));
                            m_chunks_size++;
                        }
                    }
                    else if (m_pos == m_text_length)
                    {
                        chunk.n = 2;
                        memcpy(m_chunks+m_chunks_size, &chunk,
                               sizeof(Chunk));
                        m_chunks_size++;
                    }
                    m_pos -= w2->nbytes;
                }
            }
            else if (m_pos == m_text_length)
            {
                chunk.n = 1;
                memcpy(m_chunks+m_chunks_size, &chunk, sizeof(Chunk));
                m_chunks_size++;
            }
            m_pos -= w1->nbytes;
        }

        m_pos = orig_pos;
    }

    int Algorithm::next_char()
    {
        // ONLY for UTF-8
        unsigned char ch = m_text[m_pos];
        if (ch >= 0xC0 && ch <= 0xDF)
        {
            if (m_text_length-m_pos < 2)
                return 1; /* broken text at the end */
            return 2;
        }
        if (ch >= 0xE0 && ch <= 0xEF)
        {
            if (m_text_length-m_pos < 3)
                return 1; /* broken text at the end */
            return 3;
        }
        return 1;
    }

    vector<Word *> Algorithm::find_match_words()
    {
        for (int i = 0; i < match_cache_size; ++i)
            if (m_match_cache[i].first == m_pos)
                return m_match_cache[i].second;

        vector<Word *> words;
        Word *word;
        int orig_pos = m_pos;
        int n = 0, len;

        while (m_pos < m_text_length)
        {
            len = next_char();
            if (len <= 1)
                break;
            if (n >= max_word_length())
                break;

            m_pos += len;
            n++;

            word = dict::get(m_text+orig_pos, m_pos-orig_pos);
            if (word)
                words.push_back(word);
        }

        m_pos = orig_pos;
        
        if (words.empty())
        {
            word = get_tmp_word();
            word->nbytes = next_char();
            word->length = -1;
            strncpy(word->text, m_text+m_pos, word->nbytes);
            word->text[word->nbytes] = '\0';
            words.push_back(word);
        }

        m_match_cache[m_match_cache_i] = make_pair(m_pos, words);
        m_match_cache_i++;
        if (m_match_cache_i >= match_cache_size)
            m_match_cache_i = 0;

        return words;
    }
}

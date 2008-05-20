#ifndef _WORD_H_
#define _WORD_H_

#include <climits>
#include <cstring>

#include "memory.h"

namespace rmmseg
{
    struct Word
    {
        unsigned char   nbytes;   /* number of bytes */
        unsigned char   length;   /* number of characters */
        unsigned short  freq;
        char            text[1];
    };

    /**
     * text: the text of the word.
     * length: number of characters (not bytes).
     * freq: the frequency of the word.
     */
    inline Word *make_word(const char *text, int length=1, int freq=0)
    {
        if (freq > USHRT_MAX)
            freq = USHRT_MAX;   /* avoid overflow */
        Word *w = static_cast<Word *>(pool_alloc(sizeof(Word)+strlen(text)));
        w->nbytes = std::strlen(text);
        w->length = length;
        w->freq = freq;
        std::strcpy(w->text, text);
        return w;
    }
}

#endif /* _WORD_H_ */

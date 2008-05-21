#ifndef _CHUNK_H_
#define _CHUNK_H_

#include "word.h"

namespace rmmseg
{
    /**
     * A chunk stores 3 (or less) successive words.
     */
    struct Chunk
    {
        int n;
        Word *words[3];
    };
}

#endif /* _CHUNK_H_ */

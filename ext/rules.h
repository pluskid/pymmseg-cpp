#ifndef _RULES_H_
#define _RULES_H_

#include <vector>
#include <algorithm>

#include "chunk.h"

namespace rmmseg
{
    template <typename Cmp>
    void take_highest(std::vector<Chunk> &chunks, Cmp &cmp)
    {
        int i = 1, j;
        Chunk& max = chunks[0];

        for (j = 1; j < chunks.size(); ++j)
        {
            int rlt = cmp(chunks[j], max);
            if (rlt > 0)
                i = 0;
            if (rlt >= 0)
                std::swap(chunks[i++], chunks[j]);
        }
        chunks.erase(chunks.begin()+i, chunks.end());
    }

    struct MMCmp_t
    {
        int operator()(Chunk &a, Chunk &b)
        {
            int lena=0, lenb=0;
            for (int i = 0; i < a.n; ++i)
            {
                if (a.words[i]->length == -1)
                    lena += 1;
                else
                    lena += a.words[i]->length;
            }
            for (int i = 0; i < b.n; ++i)
            {
                if (b.words[i]->length == -1)
                    lenb += 1;
                else
                    lenb += b.words[i]->length;
            }
            return lena - lenb;
        }
    } MMCmp;

    void mm_filter(std::vector<Chunk> &chunks)
    {
        take_highest(chunks, MMCmp);
    }
}

#endif /* _RULES_H_ */

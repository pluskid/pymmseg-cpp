#ifndef _RULES_H_
#define _RULES_H_

#include <vector>
#include <algorithm>

#include "chunk.h"

namespace rmmseg
{
    template <typename Cmp>
    int take_highest(std::vector<Chunk> &chunks, int n, Cmp &cmp)
    {
        unsigned int i = 1, j;
        Chunk& max = chunks[0];

        for (j = 1; j < n; ++j)
        {
            int rlt = cmp(chunks[j], max);
            if (rlt > 0)
                i = 0;
            if (rlt >= 0)
                std::swap(chunks[i++], chunks[j]);
        }
        return i;
    }

    struct MMCmp_t
    {
        int operator()(Chunk &a, Chunk &b)
        {
            return a.total_length() - b.total_length();
        }
    } MMCmp;
    int mm_filter(Chunk *chunks, int n)
    {
        return take_highest(chunks, n, MMCmp);
    }

    struct LAWLCmp_t
    {
        int operator()(Chunk &a, Chunk &b)
        {
            double rlt = a.average_length() - b.average_length();
            if (rlt == 0)
                return 0;
            if (rlt > 0)
                return 1;
            return -1;
        }
    } LAWLCmp;
    int lawl_filter(Chunk *chunks, int n)
    {
        return take_highest(chunks, n, LAWLCmp);
    }

    struct SVWLCmp_t
    {
        int operator()(Chunk &a, Chunk& b)
        {
            double rlt = a.variance() - b.variance();
            if (rlt == 0)
                return 0;
            if (rlt < 0)
                return 1;
            return -1;
        }
    } SVWLCmp;
    int svwl_filter(Chunk *chunks, int n)
    {
        return take_highest(chunks, n, SVWLCmp);
    }

    struct LSDMFOCWCmp_t
    {
        int operator()(Chunk &a, Chunk& b)
        {
            return a.degree_of_morphemic_freedom() - b.degree_of_morphemic_freedom();
        }
    } LSDMFOCWCmp;
    int lsdmfocw_filter(Chunk *chunks, int n)
    {
        return take_highest(chunks, n, LSDMFOCWCmp);
    }
}

#endif /* _RULES_H_ */

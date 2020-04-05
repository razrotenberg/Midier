#include "style.h"

#include <Arduino.h>

namespace midiate
{
namespace style
{

namespace
{

inline unsigned __factorial(unsigned x)
{
    if      (x == 0) { return 1;   }
    else if (x == 1) { return 1;   }
    else if (x == 2) { return 2;   }
    else if (x == 3) { return 6;   }
    else if (x == 4) { return 24;  }
    else if (x == 5) { return 120; }
    else if (x == 6) { return 720; }
}

// the algorithm used for permutation generation is based on algorithm 2.16 in the book
// "Combinatorial Algorithms: Generation, Enumeration, and Search" by Donald L. Kreher, Douglas R. Stinson
// the algorithm can be seen (Jan 2020) at https://cw.fel.cvut.cz/old/_media/courses/be4m33pal/lesson05_pp52_57.pdf
// book references:
//   1) https://www.amazon.com/Combinatorial-Algorithms-Enumeration-Mathematics-Applications/dp/084933988X
//   2) https://www.crcpress.com/Combinatorial-Algorithms-Generation-Enumeration-and-Search/Kreher-Stinson/p/book/9780367400156
//
void __algorithm(unsigned n, unsigned r, /* out */ unsigned * pi)
{
    pi[n-1] = 0;

    for (unsigned j = 1; j < n; ++j)
    {
        const unsigned d = (r % __factorial(j + 1)) / __factorial(j);

        r -= (d * __factorial(j));
        pi[n-j-1] = d;

        for (unsigned i = n-j; i < n; ++i)
        {
            if (pi[i] >= d)
            {
                ++pi[i];
            }
        }
    }

    // the algorithm has generated a permutation of indexes between [0,n]
    // let's now convert those indexes into chord degrees

    for (unsigned i = 0; i < n; ++i)
    {
        pi[i] = pi[i] << 1; // {0,1,2,3,..,n} -> {0,2,4,6,..,2n}

        if (pi[i] < 7)
        {
            ++pi[i]; // {0,2,4,6} -> {1,3,5,7}
        }
        else
        {
            // keep {8,10,12,..} the same as they represent {1,3,5,..}
            // but an octave higher (can be retrieved by "x-7")
        }
    }
}

} //

Degree degree(unsigned n, unsigned r, unsigned index)
{
    unsigned pi[n];

    __algorithm(n, r, /* out */ pi);

    return pi[index];
}

unsigned count(unsigned n)
{
    return __factorial(n);
}

void description(unsigned n, unsigned r, /* out */ Description & desc)
{
    unsigned pi[n];

    __algorithm(n, r, /* out */ pi);

    char * pos = desc;

    for (unsigned i = 0; i < n; ++i)
    {
        if (i > 0)
        {
            *pos++ = ' ';
        }

        itoa(pi[i], pos++, 10); // places '\0'

        if (pi[i] >= 10) // 2 digits
        {
            ++pos;
        }
    }
}

} // style
} // midiate

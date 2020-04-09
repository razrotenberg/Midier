#include "triad.h"

namespace midier
{
namespace triad
{

namespace
{

#define ASSERT(quality, expected) static_assert((int)(quality) == (int)(expected), "Expected " #quality " to be equal to " #expected);

ASSERT(Quality::m7b5,   0);
ASSERT(Quality::m7,     1);
ASSERT(Quality::dom7,   2);
ASSERT(Quality::maj7,   3);
ASSERT(Quality::aug7,   4);

ASSERT(Quality::dim,    Quality::m7b5);
ASSERT(Quality::minor,  Quality::m7);
ASSERT(Quality::major,  Quality::dom7);
ASSERT(Quality::aug,    Quality::aug7);

static const Interval __intervals[][4] =
    {
        { Interval::P1, Interval::m3, Interval::d5, Interval::m7 }, // half-diminished chord (m7b5) and diminished triad (dim)
        { Interval::P1, Interval::m3, Interval::P5, Interval::m7 }, // minor 7th chord (m7) and minor triad (minor)
        { Interval::P1, Interval::M3, Interval::P5, Interval::m7 }, // dominanth 7th chord (dom7) and major triad (major)
        { Interval::P1, Interval::M3, Interval::P5, Interval::M7 }, // major 7th (maj7)
        { Interval::P1, Interval::M3, Interval::A5, Interval::m7 }, // augmented 7th chord (aug7) and augmented triad (aug)
    };

static_assert(sizeof(__intervals) / sizeof(__intervals[0]) == 5, "Expected 5 qualities to be declared");

} //

Interval interval(Quality quality, Degree degree)
{
    Interval octaver = Interval::P1;

    if (degree > 7)
    {
        degree -= 7;
        octaver = Interval::P8;
    }

    return octaver + __intervals[(unsigned)quality][degree >> 1]; // transforming 'degree' from {1,3,5,7} to {0,1,2,3}, respectively
}

} // triad
} // midier

#include "triad.h"

namespace midiate
{
namespace triad
{

namespace
{

#define ASSERT(quality, expected) static_assert(static_cast<int>(quality) == (expected), "Expected midiate::" #quality " to be equal to " #expected);

ASSERT(Quality::m7b5,   0);
ASSERT(Quality::m7,     1);
ASSERT(Quality::dom7,   2);
ASSERT(Quality::maj7,   3);

static const Interval __intervals[][4] =
    {
        { Interval::P1, Interval::m3, Interval::d5, Interval::m7 }, // half-diminished (m7b5)
        { Interval::P1, Interval::m3, Interval::P5, Interval::m7 }, // minor 7th (m7)
        { Interval::P1, Interval::M3, Interval::P5, Interval::m7 }, // dominanth 7th (dom7)
        { Interval::P1, Interval::M3, Interval::P5, Interval::M7 }, // major 7th (maj7)
    };

static_assert(sizeof(__intervals) / sizeof(__intervals[0]) == 4, "Expected 4 qualities to be declared");

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
} // midiate

#include "triad.h"

namespace midiate
{
namespace triad
{

namespace
{

#define ASSERT(quality, expected) static_assert(static_cast<int>(quality) == (expected), "Expected midiate::" #quality " to be equal to " #expected);

ASSERT(Quality::Major,      0);
ASSERT(Quality::Minor,      1);
ASSERT(Quality::Diminished, 2);
ASSERT(Quality::Augmented,  3);

static const Interval __intervals[][3] =
    {
        { Interval::P1, Interval::M3, Interval::P5 }, // Major
        { Interval::P1, Interval::m3, Interval::P5 }, // Minor
        { Interval::P1, Interval::m3, Interval::d5 }, // Diminished
        { Interval::P1, Interval::M3, Interval::A5 }, // Augmented
    };

static_assert(sizeof(__intervals) / sizeof(__intervals[0]) == 4, "Expected 4 qualities to be declared");

} //

Interval interval(Quality quality, char degree)
{
    return __intervals[static_cast<int>(quality)][degree >> 1]; // transforming 'degree' from {1,3,5} to {0,1,2}, respectively
}

} // triad
} // midiate

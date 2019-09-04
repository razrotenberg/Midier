#include "triad.h"

namespace midiate
{

namespace
{

struct Description
{
    Interval I;
    Interval III;
    Interval V;
};

#define ASSERT(quality, expected) static_assert(static_cast<int>(quality) == (expected), "Expected midiate::" #quality " to be equal to " #expected);

ASSERT(Quality::Major,      0);
ASSERT(Quality::Minor,      1);
ASSERT(Quality::Diminished, 2);
ASSERT(Quality::Augmented,  3);

static const Description __descriptions[] =
    {
        /* Major      */ { Interval::P1, Interval::M3, Interval::P5 },
        /* Minor      */ { Interval::P1, Interval::m3, Interval::P5 },
        /* Diminished */ { Interval::P1, Interval::m3, Interval::d5 },
        /* Augmented  */ { Interval::P1, Interval::M3, Interval::A5 },
    };

static_assert(sizeof(__descriptions) / sizeof(Description) == 4, "Expected 4 qualities to be declared");

} //

#define DESCRIPTION(quality) (__descriptions[static_cast<int>(quality)])

Triad::Triad(Pitch root, Quality quality) :
    I  (root + DESCRIPTION(quality).I),
    III(root + DESCRIPTION(quality).III),
    V  (root + DESCRIPTION(quality).V)
{}

} // midiate

#include "scale.h"

namespace midiate
{

namespace
{

struct Description
{
    struct Triad
    {
        Interval interval;
        Quality  quality;
    };

    Triad I;
    Triad II;
    Triad III;
    Triad IV;
    Triad V;
    Triad VI;
    Triad VII;
    Triad VIII;
};

#define ASSERT(mode, expected) static_assert(static_cast<int>(mode) == (expected), "Expected midiate::" #mode " to be equal to " #expected);

ASSERT(Mode::Ionian,     0);
ASSERT(Mode::Dorian,     1);
ASSERT(Mode::Phrygian,   2);
ASSERT(Mode::Lydian,     3);
ASSERT(Mode::Mixolydian, 4);
ASSERT(Mode::Aeolian,    5);
ASSERT(Mode::Locrian,    6);

static const Description __descriptions[] =
    {
        {
            // Ionian
            { Interval::P1, Quality::Major      },
            { Interval::M2, Quality::Minor      },
            { Interval::M3, Quality::Minor      },
            { Interval::P4, Quality::Major      },
            { Interval::P5, Quality::Major      },
            { Interval::M6, Quality::Minor      },
            { Interval::M7, Quality::Diminished },
            { Interval::P8, Quality::Major      },
        },
        {
            // Dorian
            { Interval::P1, Quality::Minor      },
            { Interval::M2, Quality::Minor      },
            { Interval::m3, Quality::Major      },
            { Interval::P4, Quality::Major      },
            { Interval::P5, Quality::Minor      },
            { Interval::M6, Quality::Diminished },
            { Interval::m7, Quality::Major      },
            { Interval::P8, Quality::Minor      },
        },
        {
            // Phrygian
            { Interval::P1, Quality::Minor      },
            { Interval::m2, Quality::Major      },
            { Interval::m3, Quality::Major      },
            { Interval::P4, Quality::Minor      },
            { Interval::P5, Quality::Diminished },
            { Interval::m6, Quality::Major      },
            { Interval::m7, Quality::Minor      },
            { Interval::P8, Quality::Minor      },
        },
        {
            // Lydian
            { Interval::P1, Quality::Major      },
            { Interval::M2, Quality::Major      },
            { Interval::M3, Quality::Minor      },
            { Interval::A4, Quality::Diminished },
            { Interval::P5, Quality::Major      },
            { Interval::M6, Quality::Minor      },
            { Interval::M7, Quality::Minor      },
            { Interval::P8, Quality::Major      },
        },
        {
            // Mixolydian
            { Interval::P1, Quality::Major      },
            { Interval::M2, Quality::Minor      },
            { Interval::M3, Quality::Diminished },
            { Interval::P4, Quality::Major      },
            { Interval::P5, Quality::Minor      },
            { Interval::M6, Quality::Minor      },
            { Interval::m7, Quality::Major      },
            { Interval::P8, Quality::Major      },
        },
        {
            // Aeolian
            { Interval::P1, Quality::Minor      },
            { Interval::M2, Quality::Diminished },
            { Interval::m3, Quality::Major      },
            { Interval::P4, Quality::Minor      },
            { Interval::P5, Quality::Minor      },
            { Interval::m6, Quality::Major      },
            { Interval::m7, Quality::Major      },
            { Interval::P8, Quality::Minor      },
        },
        {
            // Locrian
            { Interval::P1, Quality::Diminished },
            { Interval::m2, Quality::Major      },
            { Interval::m3, Quality::Minor      },
            { Interval::P4, Quality::Minor      },
            { Interval::d5, Quality::Major      },
            { Interval::m6, Quality::Major      },
            { Interval::m7, Quality::Minor      },
            { Interval::P8, Quality::Diminished },
        },
    };

static_assert(sizeof(__descriptions) / sizeof(Description) == 7, "Expected 7 modes to be declared");

#define DESCRIPTION(mode) (__descriptions[static_cast<int>(mode)])

} //

Scale::Scale(Pitch root, Mode mode) :
    I   (root + DESCRIPTION(mode).I.interval,    DESCRIPTION(mode).I.quality),
    II  (root + DESCRIPTION(mode).II.interval,   DESCRIPTION(mode).II.quality),
    III (root + DESCRIPTION(mode).III.interval,  DESCRIPTION(mode).III.quality),
    IV  (root + DESCRIPTION(mode).IV.interval,   DESCRIPTION(mode).IV.quality),
    V   (root + DESCRIPTION(mode).V.interval,    DESCRIPTION(mode).V.quality),
    VI  (root + DESCRIPTION(mode).VI.interval,   DESCRIPTION(mode).VI.quality),
    VII (root + DESCRIPTION(mode).VII.interval,  DESCRIPTION(mode).VII.quality),
    VIII(root + DESCRIPTION(mode).VIII.interval, DESCRIPTION(mode).VIII.quality)
{}

} // midiate

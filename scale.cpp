#include "scale.h"

namespace midiate
{
namespace scale
{

namespace
{

#define ASSERT(mode, expected) static_assert(static_cast<int>(mode) == (expected), "Expected midiate::" #mode " to be equal to " #expected);

ASSERT(Mode::Ionian,     0);
ASSERT(Mode::Dorian,     1);
ASSERT(Mode::Phrygian,   2);
ASSERT(Mode::Lydian,     3);
ASSERT(Mode::Mixolydian, 4);
ASSERT(Mode::Aeolian,    5);
ASSERT(Mode::Locrian,    6);

static const Interval __intervals[][8] =
    {
        { Interval::P1, Interval::M2, Interval::M3, Interval::P4, Interval::P5, Interval::M6, Interval::M7, Interval::P8 }, // Ionian
        { Interval::P1, Interval::M2, Interval::m3, Interval::P4, Interval::P5, Interval::M6, Interval::m7, Interval::P8 }, // Dorian
        { Interval::P1, Interval::m2, Interval::m3, Interval::P4, Interval::P5, Interval::m6, Interval::m7, Interval::P8 }, // Phrygian
        { Interval::P1, Interval::M2, Interval::M3, Interval::A4, Interval::P5, Interval::M6, Interval::M7, Interval::P8 }, // Lydian
        { Interval::P1, Interval::M2, Interval::M3, Interval::P4, Interval::P5, Interval::M6, Interval::m7, Interval::P8 }, // Mixolydian
        { Interval::P1, Interval::M2, Interval::m3, Interval::P4, Interval::P5, Interval::m6, Interval::m7, Interval::P8 }, // Aeolian
        { Interval::P1, Interval::m2, Interval::m3, Interval::P4, Interval::d5, Interval::m6, Interval::m7, Interval::P8 }, // Locrian
    };

static_assert(sizeof(__intervals) / sizeof(__intervals[0]) == 7, "Expected 7 modes to be declared");

static const Quality __qualities[] = { Quality::Major, Quality::Minor, Quality::Minor, Quality::Major, Quality::Major, Quality::Minor, Quality::Diminished };

static_assert(sizeof(__qualities) / sizeof(__qualities[0]) == 7, "Expected 7 qualities to be declared");

} //

Interval interval(Mode mode, char degree)
{
    return __intervals[static_cast<int>(mode)][degree - 1];
}

Quality quality(Mode mode, char degree)
{
    return __qualities[((static_cast<int>(degree) - 1) + static_cast<int>(mode)) % (sizeof(__qualities) / sizeof(__qualities[0]))];
}

} // scale
} // midiate

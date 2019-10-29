#include "scale.h"

#include <assert.h>

namespace midiate
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

#define INTERVAL(mode, degree) (__intervals[static_cast<int>(mode)][degree - 1])

static const Quality __qualities[] = { Quality::Major, Quality::Minor, Quality::Minor, Quality::Major, Quality::Major, Quality::Minor, Quality::Diminished };

static_assert(sizeof(__qualities) / sizeof(__qualities[0]) == 7, "Expected 7 qualities to be declared");

#define QUALITY(mode, degree) (__qualities[((static_cast<int>(degree) - 1) + static_cast<int>(mode)) % (sizeof(__qualities) / sizeof(__qualities[0]))])

#define TRIAD(root, mode, degree) Triad(root + INTERVAL(mode, degree), QUALITY(mode, degree))

} //

Scale::Scale(Pitch root, Mode mode) :
    I   (TRIAD(root, mode, 1)),
    II  (TRIAD(root, mode, 2)),
    III (TRIAD(root, mode, 3)),
    IV  (TRIAD(root, mode, 4)),
    V   (TRIAD(root, mode, 5)),
    VI  (TRIAD(root, mode, 6)),
    VII (TRIAD(root, mode, 7)),
    VIII(TRIAD(root, mode, 8))
{}

const Triad & Scale::degree(int i) const
{
    if      (i == 1) { return I;    }
    else if (i == 2) { return II;   }
    else if (i == 3) { return III;  }
    else if (i == 4) { return IV;   }
    else if (i == 5) { return V;    }
    else if (i == 6) { return VI;   }
    else if (i == 7) { return VII;  }
    else if (i == 8) { return VIII; }

    assert(false);
}

} // midiate

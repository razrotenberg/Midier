#include "scale.h"

#include <Arduino.h>

namespace midier
{
namespace scale
{

#define ASSERT(mode, expected) static_assert(static_cast<int>(mode) == (expected), "Expected midier::" #mode " to be equal to " #expected);

ASSERT(Mode::Ionian,     0);
ASSERT(Mode::Dorian,     1);
ASSERT(Mode::Phrygian,   2);
ASSERT(Mode::Lydian,     3);
ASSERT(Mode::Mixolydian, 4);
ASSERT(Mode::Aeolian,    5);
ASSERT(Mode::Locrian,    6);

Interval interval(Mode mode, Degree degree)
{
    static Interval const PROGMEM __ionian     [] = { Interval::P1, Interval::M2, Interval::M3, Interval::P4, Interval::P5, Interval::M6, Interval::M7 };
    static Interval const PROGMEM __dorian     [] = { Interval::P1, Interval::M2, Interval::m3, Interval::P4, Interval::P5, Interval::M6, Interval::m7 };
    static Interval const PROGMEM __phrygian   [] = { Interval::P1, Interval::m2, Interval::m3, Interval::P4, Interval::P5, Interval::m6, Interval::m7 };
    static Interval const PROGMEM __lydian     [] = { Interval::P1, Interval::M2, Interval::M3, Interval::A4, Interval::P5, Interval::M6, Interval::M7 };
    static Interval const PROGMEM __mixolydian [] = { Interval::P1, Interval::M2, Interval::M3, Interval::P4, Interval::P5, Interval::M6, Interval::m7 };
    static Interval const PROGMEM __aeolian    [] = { Interval::P1, Interval::M2, Interval::m3, Interval::P4, Interval::P5, Interval::m6, Interval::m7 };
    static Interval const PROGMEM __locrian    [] = { Interval::P1, Interval::m2, Interval::m3, Interval::P4, Interval::d5, Interval::m6, Interval::m7 };

    static Interval const * const __all[] PROGMEM =
        {
            __ionian,
            __dorian,
            __phrygian,
            __lydian,
            __mixolydian,
            __aeolian,
            __locrian,
        };

    static_assert(sizeof(__all) / sizeof(__all[0]) == (unsigned)Mode::Count, "Unexpected number of modes declared");
    static_assert(sizeof(Interval) == sizeof(byte), "Unexpected size of 'Interval'");

    Interval octaver = Interval::P1;

    while (degree > 7)
    {
        degree -= 7;
        octaver = octaver + Interval::P8;
    }

    return octaver + (Interval)pgm_read_byte(pgm_read_ptr(__all + (unsigned)mode) + (degree - 1));
}

Quality quality(Mode mode, Degree degree)
{
    static Quality const PROGMEM __qualities[] = { Quality::maj7, Quality::m7, Quality::m7, Quality::maj7, Quality::dom7, Quality::m7, Quality::m7b5 };

    constexpr auto __count = sizeof(__qualities) / sizeof(__qualities[0]);

    static_assert(__count == 7, "Expected 7 qualities to be declared");
    static_assert(sizeof(Quality) == sizeof(byte), "Unexpected size of 'Quality'");

    return (Quality)pgm_read_byte(__qualities + ((degree - 1 + (unsigned)mode) % __count));
}

} // scale
} // midier

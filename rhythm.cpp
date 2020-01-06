#include "rhythm.h"

#include "debug.h"
#include "layer.h"
#include "time.h"

#include <Arduino.h>
#include <avr/pgmspace.h>

namespace midiate
{
namespace rhythm
{

namespace
{

inline float fractional(float x)
{
    return x - (long)x;
}

inline unsigned integer(float x)
{
    return x;
}

} //

namespace descriptions
{

const Description PROGMEM _0  = "[*           ]";
const Description PROGMEM _1  = "[*     *     ]";
const Description PROGMEM _2  = "[*  *  *  *  ]";
const Description PROGMEM _3  = "[*  *  *     ]";
const Description PROGMEM _4  = "[*     *  *  ]";
const Description PROGMEM _5  = "[*  *     *  ]";
const Description PROGMEM _6  = "[   *  *  *  ]";
const Description PROGMEM _7  = "[*   *   *   ]";
const Description PROGMEM _8  = "[*       *   ]";
const Description PROGMEM _9  = "[*      (*)  ]";
const Description PROGMEM _10 = "[* * * * * * ]";

char const * const all[] PROGMEM = {
    _0,
    _1,
    _2,
    _3,
    _4,
    _5,
    _6,
    _7,
    _8,
    _9,
    _10,
};

static_assert(sizeof(all) / sizeof(all[0]) == (unsigned)Rhythm::Count, "Unexxpected number of descriptions declared");

} // descriptions

namespace rhythmers
{

bool generic(const float portions[], unsigned count, const Layer & layer, const Time & now, /* out */ unsigned & index)
{
    unsigned length = 1; // # of bars in the rhythm
    for (unsigned i = 0; i < count; ++i)
    {
        length = max(length, integer(portions[i]) + 1);
    }

    const auto difference = now - layer.start;

    for (unsigned i = 0; i < count; ++i)
    {
        if (difference.subdivisions != (char)(fractional(portions[i]) * Time::Subdivisions))
        {
            continue;
        }

        if ((layer.bar % length) != integer(portions[i]))
        {
            continue;
        }

        /* out */ index = i + ((layer.bar / length) * count);

        TRACE_6("Playing note #", index, " of layer ", layer, " at beat ", now);

        return true;
    }

    return false;
}

#define RHYTHMER(...)                                                               \
    [](const Layer & layer, const Time & now, /* out */ unsigned & index) -> bool   \
    {                                                                               \
        const float __portions[] = { __VA_ARGS__ };                                 \
        const unsigned __count = sizeof(__portions) / sizeof(__portions[0]);        \
                                                                                    \
        static_assert(120 % __count == 0, "Invalid # of rhythm notes");             \
                                                                                    \
        return generic(__portions, __count, layer, now, /* out */ index);           \
    }

#define _1_1(i) (i / 1.f)
#define _1_2(i) (i / 2.f)
#define _1_3(i) (i / 3.f)
#define _1_4(i) (i / 4.f)
#define _1_6(i) (i / 6.f)

using rhythmer_t = bool(*)(const Layer &, const Time &, /* out */ unsigned &);

const rhythmer_t all[] =
    {
        RHYTHMER(_1_1(0)),                            // 1/4                        [*           ]
        RHYTHMER(_1_2(0),          _1_2(1)),          // 1/8       1/8              [*     *     ]
        RHYTHMER(_1_4(0), _1_4(1), _1_4(2), _1_4(3)), // 1/16 1/16 1/16 1/16        [*  *  *  *  ]
        RHYTHMER(_1_4(0), _1_4(1), _1_4(2)),          // 1/16 1/16 1/8              [*  *  *     ]
        RHYTHMER(_1_4(0),          _1_4(2), _1_4(3)), // 1/8       1/16 1/16        [*     *  *  ]
        RHYTHMER(_1_4(0), _1_4(1),          _1_4(3)), // 1/16 1/8       1/16        [*  *     *  ]
        RHYTHMER(         _1_4(1), _1_4(2), _1_4(3)), //      1/16 1/16 1/16        [   *  *  *  ]
        RHYTHMER(_1_3(0),      _1_3(1),     _1_3(2)), // 1/8th note triplet         [*   *   *   ]
        RHYTHMER(_1_3(0),                   _1_3(2)), // 1/8th note swung triplet   [*       *   ]
        RHYTHMER(_1_3(0), _1_3(3),          _1_3(5)), // 2-bar 1/8th note swing     [*      (*)  ]
        RHYTHMER(                                     // 1/16th note sextuplet      [* * * * * * ]
            _1_6(0), _1_6(1), _1_6(2), _1_6(3), _1_6(4), _1_6(5)),
    };

static_assert(sizeof(all) / sizeof(all[0]) == (unsigned)Rhythm::Count, "Unexxpected number of rhythms declared");

} // rhythmers

void description(Rhythm rhythm, /* out */ Description & desc)
{
    strcpy_P(
        /* out */ desc,
        (char const *)pgm_read_ptr(&(descriptions::all[(unsigned)rhythm]))
    );
}

bool played(Rhythm rhythm, const Layer & layer, const Time & now, /* out */ unsigned & index)
{
    return rhythmers::all[(unsigned)rhythm](layer, now, /* out */ index);
}

} // rhythm
} // midiate

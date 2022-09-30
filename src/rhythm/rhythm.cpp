#include "rhythm.h"

#include "../debug/debug.h"
#include "../layer/layer.h"
#include "../time/time.h"

#if defined(ARDUINO)
#include <Arduino.h>
#else
#include <string.h>
#define max(a,b) ((a)>(b)?(a):(b))
#define PROGMEM // nothing
#endif

namespace midier
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
const Description PROGMEM _11 = "[*   * *   * ]";

} // descriptions

namespace portions
{

#define _1_1(i) (i / 1.f)
#define _1_2(i) (i / 2.f)
#define _1_3(i) (i / 3.f)
#define _1_4(i) (i / 4.f)
#define _1_6(i) (i / 6.f)

const float PROGMEM _0  [] = { _1_1(0)                              };  // 1/4                        [*           ]
const float PROGMEM _1  [] = { _1_2(0),          _1_2(1)            };  // 1/8       1/8              [*     *     ]
const float PROGMEM _2  [] = { _1_4(0), _1_4(1), _1_4(2), _1_4(3)   };  // 1/16 1/16 1/16 1/16        [*  *  *  *  ]
const float PROGMEM _3  [] = { _1_4(0), _1_4(1), _1_4(2)            };  // 1/16 1/16 1/8              [*  *  *     ]
const float PROGMEM _4  [] = { _1_4(0),          _1_4(2), _1_4(3)   };  // 1/8       1/16 1/16        [*     *  *  ]
const float PROGMEM _5  [] = { _1_4(0), _1_4(1),          _1_4(3)   };  // 1/16 1/8       1/16        [*  *     *  ]
const float PROGMEM _6  [] = {          _1_4(1), _1_4(2), _1_4(3)   };  //      1/16 1/16 1/16        [   *  *  *  ]
const float PROGMEM _7  [] = { _1_3(0),      _1_3(1),     _1_3(2)   };  // 1/8th note triplet         [*   *   *   ]
const float PROGMEM _8  [] = { _1_3(0),                   _1_3(2)   };  // 1/8th note swung triplet   [*       *   ]
const float PROGMEM _9  [] = { _1_3(0), _1_3(3),          _1_3(5)   };  // 2-bar 1/8th note swing     [*      (*)  ]
const float PROGMEM _10 [] =                                            // 1/16th note sextuplet      [* * * * * * ]
            { _1_6(0), _1_6(1), _1_6(2), _1_6(3), _1_6(4), _1_6(5) };
const float PROGMEM _11 [] =                                            // 1/16th note swung tuplet   [*   * *   * ]
            { _1_6(0),          _1_6(2), _1_6(3),          _1_6(5) };

}

struct Rhythmer
{
    void description(/* out */ Description & out) const
    {
#if defined(ARDUINO)
        strcpy_P(
            /* out */ out,
            (char const *)pgm_read_ptr(&(this->_description))
        );
#else
        strcpy(/* out */ out, _description);
#endif
    }

    Rate rate() const
    {
#if defined(ARDUINO)
        return (Rate)pgm_read_byte(&(this->_rate));
#else
        return _rate;
#endif
    }

    unsigned count() const
    {
#if defined(ARDUINO)
        return (unsigned)pgm_read_byte(&(this->_count));
#else
        return _count;
#endif
    }

    float portion(unsigned index) const
    {
#if defined(ARDUINO)
        return pgm_read_float((float const *)pgm_read_ptr(&(this->_portions)) + index);
#else
        return _portions[index];
#endif
    }

    // members should not be accessed directly as they hold addresses to PROGMEM
    const char * _description;
    Rate _rate;
    unsigned char _count;
    const float * _portions;
};

#define RHYTHMER(__i, __rate) { descriptions:: __i, __rate, (sizeof(portions:: __i) / sizeof(portions:: __i[0])), portions::__i }

const Rhythmer __rhythmers[] PROGMEM =
    {
        RHYTHMER(_0,  Rate::_1_4),
        RHYTHMER(_1,  Rate::_1_8),
        RHYTHMER(_2,  Rate::_1_16),
        RHYTHMER(_3,  Rate::_1_16),
        RHYTHMER(_4,  Rate::_1_16),
        RHYTHMER(_5,  Rate::_1_16),
        RHYTHMER(_6,  Rate::_1_16),
        RHYTHMER(_7,  Rate::triplet),
        RHYTHMER(_8,  Rate::triplet),
        RHYTHMER(_9,  Rate::triplet),
        RHYTHMER(_10, Rate::sextuplet),
        RHYTHMER(_11, Rate::sextuplet),
    };

static_assert(sizeof(__rhythmers) / sizeof(__rhythmers[0]) == (unsigned)Rhythm::Count, "Unexpected number of rhythmers declared");

} //

void description(Rhythm rhythm, /* out */ Description & desc)
{
    __rhythmers[(unsigned)rhythm].description(/* out */ desc);
}

bool played(Rhythm rhythm, const Layer & layer, /* out */ unsigned & index)
{
    const Rhythmer & rhythmer = __rhythmers[(unsigned)rhythm];

    const auto count = rhythmer.count();

    unsigned length = 1; // # of bars in the rhythm
    for (unsigned i = 0; i < count; ++i)
    {
        length = max(length, integer(rhythmer.portion(i)) + 1);
    }

    const auto difference = Time::now - layer.start;

    for (unsigned i = 0; i < count; ++i)
    {
        const auto portion = rhythmer.portion(i);

        if (difference.subdivisions != (char)(fractional(portion) * Time::Subdivisions))
        {
            continue;
        }

        if ((layer.bar % length) != integer(portion))
        {
            continue;
        }

        /* out */ index = i + ((layer.bar / length) * count);

        TRACE_4(F("Playing note #"), index, F(" of layer "), layer);

        return true;
    }

    return false;
}

Rate rate(Rhythm rhythm)
{
    return __rhythmers[(unsigned)rhythm].rate();
}

} // rhythm
} // midier

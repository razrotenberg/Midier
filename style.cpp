#include "style.h"

#include <avr/pgmspace.h>

namespace midiate
{
namespace style
{

namespace
{

#define STYLER(...)                                                         \
    [](unsigned index) -> Degree                                            \
    {                                                                       \
        constexpr Degree __notes[] = { __VA_ARGS__ };                       \
        constexpr unsigned __count = sizeof(__notes) / sizeof(__notes[0]);  \
                                                                            \
        static_assert(60 % __count == 0, "Invalid # of style notes");       \
                                                                            \
        return __notes[index % __count];                                    \
    }

using styler_t = Degree(*)(unsigned);

const styler_t __stylers[] =
    {
        STYLER(1, 3, 5),
        STYLER(5, 3, 1),
        STYLER(1, 3, 5, 3),
        STYLER(5, 3, 1, 3),
        STYLER(1, 3, 5, 3, 1),
        STYLER(5, 3, 1, 3, 5),
        STYLER(1, 5, 3),
        STYLER(3, 1, 5),
        STYLER(1, 5, 3, 5),
        STYLER(1, 3, 1, 5),
        STYLER(1, 3, 1, 5, 1, 3),
        STYLER(5, 3, 5, 1, 5, 3),
    };

static_assert(sizeof(__stylers) / sizeof(__stylers[0]) == (unsigned)Style::Count, "Unexpected number of styles declared");

const Name PROGMEM __up = "Up";
const Name PROGMEM __down = "Down";
const Name PROGMEM __up_down = "Up Down";
const Name PROGMEM __down_up = "Down Up";
const Name PROGMEM __up_and_down = "Up and Down";
const Name PROGMEM __down_and_up = "Down and Up";
const Name PROGMEM __converge = "Converge";
const Name PROGMEM __diverge = "Diverge";
const Name PROGMEM __pinky_up = "Pinky Up";
const Name PROGMEM __thumb_up = "Thumb Up";
const Name PROGMEM __1_3_1_5_1_3 = "";
const Name PROGMEM __5_3_5_1_5_3 = "";

char const * const __names[] PROGMEM = {
    __up,
    __down,
    __up_down,
    __down_up,
    __up_and_down,
    __down_and_up,
    __converge,
    __diverge,
    __pinky_up,
    __thumb_up,
    __1_3_1_5_1_3,
    __5_3_5_1_5_3,
};

static_assert(sizeof(__names) / sizeof(__names[0]) == (unsigned)Style::Count, "Unexpected number of names declared");

} //

Degree degree(Style style, unsigned index)
{
    return __stylers[(unsigned)style](index);
}

void name(Style style, /* out */ Name & name)
{
    strcpy_P(
        /* out */ name,
        (char const *)pgm_read_ptr(&(__names[(unsigned)style]))
    );
}

} // style
} // midiate

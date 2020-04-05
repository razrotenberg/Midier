#include "mode.h"

#include <avr/pgmspace.h>

namespace midiate
{
namespace mode
{

namespace
{

const Name PROGMEM __ionian = "Ionian";
const Name PROGMEM __dorian = "Dorian";
const Name PROGMEM __phrygian = "Phrygian";
const Name PROGMEM __lydian = "Lydian";
const Name PROGMEM __mixolydian = "Mixolydian";
const Name PROGMEM __aeolian = "Aeolian";
const Name PROGMEM __locrian = "Locrian";

char const * const __names[] PROGMEM = {
    __ionian,
    __dorian,
    __phrygian,
    __lydian,
    __mixolydian,
    __aeolian,
    __locrian,
};

static_assert(sizeof(__names) / sizeof(__names[0]) == (unsigned)Mode::Count, "Unexpected number of names declared");

} //

void name(Mode mode, /* out */ Name & name)
{
    strcpy_P(
        /* out */ name,
        (char const *)pgm_read_ptr(&(__names[(unsigned)mode]))
    );
}

} // mode
} // midiate

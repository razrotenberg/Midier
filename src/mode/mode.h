#pragma once

namespace midiate
{

enum class Mode : char
{
    Ionian,
    Dorian,
    Phrygian,
    Lydian,
    Mixolydian,
    Aeolian,
    Locrian,

    // last element to indicate the # of actual elements
    Count,
};

namespace mode
{

using Name = char[11]; // "mixolydian" + '\0'

void name(Mode mode, /* out */ Name & name);

} // mode

} // midiate

#pragma once

namespace midier
{

enum class Rate : char
{
    // values are the number of units that exist in a whole bar
    // bars are in 1/4 time signature

    _1_4            = 1,
    _1_8            = 2,
    _1_16           = 4,
    _1_8_triplet    = 3,
    _1_16_triplet   = 6,
};

enum class Rhythm : char
{
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

    // last element to indicate the # of actual elements
    Count,
};

struct Layer;
struct Time;

namespace rhythm
{

using Description = char[15]; // '[' + 12 + ']' + '\0'

void description(Rhythm rhythm, /* out */ Description & desc);
bool played(Rhythm rhythm, const Layer & layer, /* out */ unsigned & index);
Rate rate(Rhythm rhythm);

} // rhythm

} // midier

#pragma once

namespace midier
{

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

} // rhythm

} // midier

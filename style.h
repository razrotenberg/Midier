#pragma once

#include "degree.h"

namespace midiate
{

#define ALIASED(name, alias) name, alias = name

enum class Style : char
{
    // inspired by Ableton Live's arpeggiator
    ALIASED(_1_3_5,     Up),
    ALIASED(_5_3_1,     Down),
    ALIASED(_1_3_5_3,   UpDown),
    ALIASED(_5_3_1_3,   DownUp),
    ALIASED(_1_3_5_3_1, UpAndDown),
    ALIASED(_5_3_1_3_5, DownAndUp),
    ALIASED(_1_5_3,     Converge),
    ALIASED(_3_1_5,     Diverge),
    ALIASED(_1_5_3_5,   PinkyUp),
    ALIASED(_1_3_1_5,   ThumbUp),

    // more styles
    _1_3_1_5_1_3,
    _5_3_5_1_5_3,

    // last element to indicate the # of actual elements
    Count,
};

#undef ALIASED

namespace style
{

Degree degree(Style style, unsigned index);

} // style

} // midiate

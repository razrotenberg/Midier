#pragma once

#include "sequence.h"

namespace midiate
{

struct Moment
{
    char subdivision = -1; // can be 'char' as long as Beat::Subdivisions is less than 127
    long bars = 0; // a 32-bit mask to represent the bars a moment should be played in
};

template<>
inline bool Valid<Moment>(const Moment & moment)
{
    return moment.subdivision != -1;
}

} // midiate

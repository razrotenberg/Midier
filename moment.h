#pragma once

#include "marker.h"

namespace midiate
{

struct Moment
{
    char subdivision = 0; // can be 'char' as long as Beat::Subdivisions is less than 127
    long bars = 0; // a 32-bit mask to represent the bars a moment should be played in
};

template<>
struct Marker<Moment>
{
    static void end(Moment & moment)
    {
        moment.subdivision = -1;
    }

    static bool ended(const Moment & moment)
    {
        return moment.subdivision == -1;
    }
};

} // midiate

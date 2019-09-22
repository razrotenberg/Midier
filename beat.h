#pragma once

#include "sequence.h"

namespace midiate
{

struct Beat
{
    union
    {
        // the bar index within a loop
        char index = -1;
        
        // a bit mask representing in which bars a beat should be played in a loop
        // 32-bit as we support maximum of 32 bars in a loop
        long mask;
    } bar;
    
    char subdivision = 0; // can be 'char' as long as Beat::Subdivisions is less than 127

    Beat & operator++();

    // bars are in 1/4 time signature
    // we support the following note values:
    //   1) 1/16 (4 notes in a bar)
    //   2) 1/8 (2 notes in a bar)
    //   3) 1/8 triplet (3 notes in a bar)
    // therefore, we use a multiply of 12 for the number of subdivisions in a bar,
    // as it gives us the needed rhythmic precision
    // we use 96 in order to shorten the delay after every subdivision played
    //
    constexpr static auto Subdivisions = 96;
};

Beat operator+(const Beat & beat, int subdivisions);

template<>
struct Traits<Beat>
{
    static void end(Beat & beat)
    {
        beat.subdivision = -1;
    }

    static bool ended(const Beat & beat)
    {
        return beat.subdivision == -1;
    }
};

} // midiate

#pragma once

#include "beat.h"
#include "sequence.h"
#include "triad.h"

namespace midiate
{

enum class Style : char
{
    // inspired by Ableton Live's arpeggiator

    Up,
    Down,
    UpDown,
    DownUp,
    UpAndDown,
    DownAndUp,
    Converge,
    Diverge,
    PinkyUp,
    ThumbUp,
};

enum class Rhythm : char
{
    A,  // 1/16 1/16 1/16 1/16          [*  *  *  *  ]
    B,  // 1/16 1/16 1/8                [*  *  *     ]
    C,  // 1/8       1/16 1/16          [*     *  *  ]
    D,  // 1/16 1/8       1/16          [*  *     *  ]
    E,  //      1/16 1/16 1/16          [   *  *  *  ]
    F,  // 1/8th note triplet           [*   *   *   ]
    G,  // 1/8th note swuang triplet    [*       *   ]
};

struct Layer
{
    Layer() = default;
    Layer(const Triad & triad, Style style, Rhythm rhythm, const Beat & start, char tag);

    void play(const Beat & now); // (maybe) play the next pitch

    char tag = -1;

    constexpr static auto Pitches = 5;
    constexpr static auto Beats   = 4;

private:
    Sequence<Pitch, Pitches> _pitches;
    Sequence<Beat,  Beats>   _beats;
};

} // midiate

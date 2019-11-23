#pragma once

#include "beat.h"
#include "moment.h"
#include "pitch.h"
#include "rhythm.h"
#include "sequence.h"
#include "style.h"

namespace midiate
{

struct Layer
{
    enum class State : char
    {
        Wander,
        Record,
        Playback,
    };

    Layer() = default;
    Layer(char degree, Style style, Rhythm rhythm, const Time & now, char tag);

    bool play(const Time & now, /* out */ Pitch & pitch); // get the pitch that should be played right now, if there is any
    void reset(); // reset both '_pitches' and '_moments'

    char tag = -1;
    Time start; // the starting time in a loop (irrelevant for wander mode)
    State state;

    constexpr static auto Pitches = 5;
    constexpr static auto Moments = 4;

private:
    Sequence<Pitch,  Layer::Pitches> _pitches;
    Sequence<Moment, Layer::Moments> _moments;
};

} // midiate

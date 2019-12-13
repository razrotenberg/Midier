#pragma once

#include "time.h"
#include "pitch.h"
#include "rhythm.h"
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
    Layer(char tag, Degree chord, const Time & now);

    bool play(const Time & now, Style style, Rhythm rhythm, /* out */ Pitch & pitch); // get the pitch that should be played right now, if there is any
    
    char   tag   = -1;
    State  state = State::Wander;
    Degree chord;
    Time   start;

    struct {
        Time start;
        Time end;
    } record;
};

} // midiate

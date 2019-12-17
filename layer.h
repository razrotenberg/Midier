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

    void record(const Time & now);
    void playback(const Time & now);
    void click(const Time & now);
    void revoke();
    
    bool play(const Time & now, Style style, Rhythm rhythm, /* out */ Pitch & pitch); // get the pitch that should be played right now, if there is any
    
    char tag = -1;
    State state = State::Wander;
    Degree chord;
    Time start;

    struct {
        Time start;
        Time end;
        char bar; // the bar index in which we recorded
    } loop;

    // to support resetting the bar index while keeping everything continuous,
    // we are looking for a value that will be divisible by all # of elements
    // in any style and rhythm (2,3,4,5), and we come up with 60.
    // we then multiply it by the max # of bars in a rhythm (2) and get 120.
    constexpr static auto Period = 120;
    
    // the index of the bar within the logical loop it's being played in
    char bar = 0; // can be 'char' as long as 'Layer::Period' is less than 127
};

} // midiate

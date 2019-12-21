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
        short bar; // the bar index in which we recorded
    } loop;

    // Layer::Period is the number represents the periodity of the bar index. the bar index is incremented
    // every time a full bar has passed since the layer was first started. for finite layers, the bar index
    // is set to (-1) when leaving the recorded area, and is reset to 0 when we re-enter the recorded area.
    // for infinite layers, the bar index is never (logically) reset to 0. in order to support infinite layers,
    // we must be able to cyclically increment and reset the bar index once in reaches a certain value. this
    // is the value. the bar index is only used as an argument to the rhythmer, which outputs the index of the
    // note that is being played, since the beginning of time, and this note index is being used as an argument
    // for the styler, which outputs the chord degree that should be played. we support styles with periodity
    // of 1/2/3/4/5/6 notes, therefore we want the note index - which is the output of the rhythmer, and an input
    // for the styler - to be with periodity of any multiplication of 60, so it would be divisible by all possible
    // numbers of notes in all styles. therefore, we have to make the rhythmer output indexes with such periodity.
    // the following formula descibes the output (y) of the rhythmer: y = i + (b/l) * c. where 'i' is the note
    // index within the rhythm, 'b' is the bar index, 'l' is the number of bars a rhythm is spanned over, and
    // 'c' is the number of notes played in the rhythm (over 'l' bars). the following table show the values
    // of those variables every click:
    // 
    // i: 0, 1, ... c-1, 0, 1, ... c-1, 0,  1, ... c-1, ... 0   ... 0     ... 0
    // b: 0, ?, ... ?,   l, ?, ... ?,   2l, ?, ... ?,   ... B   ... 2B    ... 4B
    // y: 0, 1, ...                                         60x ... 60x*2 ... 60x*4
    // 
    // we are looking for 'B', and we know that, when 'b' will be equal to 'B':
    // 1) y = i + B/l * c = B/l * c (because 'i' must be 0)
    // 2) y = 60 * x
    // from (1) and (2) we get: B/l * c = 60 * x -> B = 60 * x * l / c
    // 
    // 'B' has to be an integer, and if we choose 'x' to be equal to 'c', 'B' is guaranteed to be an integer,
    // and 'y' will have the periodity of 60 * c, which is good for us (as it's a multiplication of 60).
    // the (currently) supported values for 'l' are 1/2/3/4, and therefore we have to find a number that
    // is a multiplication of 60, 120, 180, 240, and we get 720.
    // 
    constexpr static auto Period = 720;
    
    // the index of the bar within the logical loop it's being played in
    short bar = 0; // must be of a big enough size in order to hold Layer::Period
};

} // midiate

#pragma once

#include "../degree/degree.h"
#include "../mode/mode.h"
#include "../note/note.h"
#include "../octave/octave.h"
#include "../rhythm/rhythm.h"
#include "../time/time.h"

namespace midiate
{

struct Layer
{
    struct Config
    {
        Note       note       = Note::C;
        Accidental accidental = Accidental::Natural;
        Octave     octave     = 3;
        Mode       mode       = Mode::Ionian;
        Rhythm     rhythm     = Rhythm::_7;

        struct
        {
            char     steps  = 3;
            unsigned perm   = 0;
            bool     looped = false;
        } style;
    };

    enum class Configured
    {
        Static,
        Dynamic,
    };

    enum class State : char
    {
        Wait,
        Wander,
        Record,
        Playback,
    };

    Layer() = default;
    Layer(char tag, Degree chord, const Time & start);

    void record();
    void playback();
    void click();
    void revoke();
    bool played(); // should the layer be played now?

    char tag = -1;
    State state;
    Degree chord;
    Time start;

    struct {
        Time start;
        Time end;
        short bar = -1; // the bar index in which we recorded
    } loop;

    Config config; // used only if statically configured
    Configured configured = Configured::Dynamic;

    // Layer::Period is the number represents the periodity of the bar index. the bar index is incremented
    // every time a full bar has passed since the layer was first started. for finite layers, the bar index
    // is set to (-1) when leaving the recorded area, and is reset to 0 when we re-enter the recorded area.
    // for infinite layers, the bar index is (logically) never reset to 0. in order to support infinite layers,
    // we must be able to cyclically increment and reset the bar index once in reaches a certain value. this
    // is the value. the bar index is only used as an argument to the rhythmer, which outputs the index of the
    // note that is being played, since the beginning of time, and this note index is being used as an argument
    // for the styler, which outputs the chord degree that should be played. we support styles with periodity
    // of 3/4/5/6 notes (4/6/8/10 when looped), therefore we want the note index - which is the output of the rhythmer,
    // and an input for the styler - to be with periodity of any multiplication of 120, so it would be divisible by
    // all possible numbers of notes in all styles. therefore, we have to make the rhythmer output indexes with
    // such periodity. the following formula descibes the output (y) of the rhythmer: y = i + (b/l) * c. where 'i'
    // is the note index within the rhythm, 'b' is the bar index, 'l' is the number of bars a rhythm is spanned over,
    // and 'c' is the number of notes played in the rhythm (over 'l' bars). the following table show the values
    // of those variables every click:
    //
    // i: 0, 1, ... c-1, 0, 1, ... c-1, 0,  1, ... c-1, ... 0    ... 0      ... 0
    // b: 0, ?, ... ?,   l, ?, ... ?,   2l, ?, ... ?,   ... B    ... 2B     ... 4B
    // y: 0, 1, ...                                         120x ... 120x*2 ... 120x*4
    //
    // we are looking for 'B', and we know that, when 'b' will be equal to 'B':
    // 1) y = i + B/l * c = B/l * c (because 'i' must be 0)
    // 2) y = 120 * x
    // from (1) and (2) we get: B/l * c = 120 * x -> B = 120 * x * l / c
    //
    // 'B' has to be an integer, and if we choose 'x' to be equal to 'c', 'B' is guaranteed to be an integer,
    // and 'y' will have the periodity of 120 * c, which is good for us (as it's a multiplication of 120).
    // the (currently) supported values for 'l' are 1/2/3/4, and therefore we have to find a number that
    // is a multiplication of 120, 240, 360, 480 and we get 1440.
    //
    constexpr static auto Period = 1440;

    // the index of the bar within the logical loop it's being played in
    short bar = 0; // must be of a big enough size in order to hold Layer::Period
};

} // midiate

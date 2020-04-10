#pragma once

#include "../degree/degree.h"
#include "../layer/layer.h"

namespace midier
{

struct Looper
{
    enum class State : char
    {
        Wander,
        Prerecord,
        Record,
        Playback,
        Overlay,
    };

    enum class Assist : char
    {
        No,

        // numbers represeting the # of subdivisions to round
        _12 = 12,
        _8 = 8,
        _6 = 6,
        _4 = 4,
        _3 = 3,
    };

    char start(Degree degree); // return corresponding tag of (-1) if could not play
    void stop(char tag);
    void revoke(char tag = -1); // (-1) means the last recorded layer

    using callback_t = void(*)(int); // (-1) means to stop counting bars

    // start the run loop and fire 'callback' for every beginning of a bar
    void run(callback_t callback);

    State state = State::Wander;
    unsigned bpm = 60;
    Assist assist = Assist::No;
    Layer layers[16];
    Time recorded;
    Time started; // first note ever played
    char bars = 0; // # of recorded bars
};

} // midier

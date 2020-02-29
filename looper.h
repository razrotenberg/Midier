#pragma once

#include "degree.h"
#include "layer.h"

namespace midiate
{

struct Looper
{
    enum class State : char
    {
        Wander,
        Record,
        Playback,
        Overlay,
    };

    char start(Degree degree); // return corresponding tag of (-1) if could not play
    void stop(char tag);
    void revoke(char tag = -1); // (-1) means the last recorded layer

    using callback_t = void(*)(int); // (-1) means to stop counting bars

    // start the run loop and fire 'callback' for every beginning of a bar
    void run(callback_t callback);

    State state = State::Wander;
    Layer::Config config;
    unsigned bpm = 60;
    Layer layers[16];
    Time beat;
    Time recorded;
    char bars = 0; // # of recorded bars
};

} // midiate

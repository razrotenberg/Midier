#pragma once

#include "scale.h"
#include "layer.h"

namespace midiate
{

struct Looper
{
    struct Config
    {
        Scale  scale;
        int    bpm;
        Style  style;
        Rhythm rhythm;
    };

    enum class State : char
    {
        Wander,
        Record,
        Playback,
        Overlay,
    };

    Looper(const Config & config);

    State state()            { return _state;  } // getter
    void  state(State state) { _state = state; } // setter

    char start(int degree);
    void stop(char tag);
    void undo(); // stop the last recorded layer

    using callback_t = void(*)(int); // (-1) means to stop counting bars

    // start the run loop and fire 'callback' for every beginning of a bar
    void run(callback_t callback);
    
private:
    const Config & _config;
    Layer _layers[16];
    Beat _beat;
    State _state;
    char _bars; // # of recorded bars
};

} // midiate

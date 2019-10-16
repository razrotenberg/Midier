#pragma once

#include "scale.h"
#include "layer.h"

namespace midiate
{

struct Looper
{
    struct Config
    {
        Note        note;
        Accidental  accidental;
        Octave      octave;
        Mode        mode;
        int         bpm;
        Style       style;
        Rhythm      rhythm;
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

    void config(const Config & config); // set the configuration

    char start(int degree);
    void stop(char tag);

    using callback_t = void(*)(int);

    // start the run loop and fire 'callback' for every beginning of a bar
    void run(callback_t callback = nullptr);
    
private:
    Config _config;
    Scale _scale;
    Layer _layers[16];
    Beat _beat;
    State _state;
    char _bars; // # of recorded bars
};

} // midiate

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
        // volume
        // inversion
    };

    enum class State : char
    {
        Wander,
        Record,
        Playback,
        Overlay,
    };

    Looper(const Config & config);

    void config(const Config & config); // set the configuration

    // play and stop playing triads in the scale
    char start(int degree); // returns the tag of the new layer or (-1) if no place for it
    void stop(char tag);

    using callback_t = void(*)(int); // (-1) means we are not in a loop

    // start the run loop and fire 'callback' for every beginning of a bar
    void run(callback_t callback = nullptr);
    
    // enter specific states
    void record();
    void playback();
    void overlay();

    State state() const;

private:
    Config _config;
    Scale _scale;
    Layer _layers[16];
    State _state = State::Wander;
    Beat _beat;
    char _bars = 0; // # of recorded bars
};

} // midiate

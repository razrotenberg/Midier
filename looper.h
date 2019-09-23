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

    Looper(const Config & config);

    void config(const Config & config); // set the configuration

    char start(int degree);
    void stop(char tag);

    using callback_t = void(*)(int); // (-1) means we are not in a loop

    // start the run loop and fire 'callback' for every beginning of a bar
    void run(callback_t callback = nullptr);
    
private:
    Config _config;
    Scale _scale;
    Layer _layers[16];
    Beat _beat;
};

} // midiate

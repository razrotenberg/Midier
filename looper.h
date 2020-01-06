#pragma once

#include "degree.h"
#include "layer.h"
#include "mode.h"
#include "note.h"
#include "octave.h"
#include "rhythm.h"
#include "style.h"

namespace midiate
{

struct Looper
{
    struct Config
    {
        Note       note;
        Accidental accidental;
        Octave     octave;
        Mode       mode;
        int        bpm;
        Rhythm     rhythm;
        char       steps;
        unsigned   perm;
        bool       looped;
    };

    enum class State : char
    {
        Wander,
        Record,
        Playback,
        Overlay,
    };

    Looper(const Config & config);

    char start(Degree degree); // return corresponding tag of (-1) if could not play
    void stop(char tag);
    void undo(); // stop the last recorded layer

    using callback_t = void(*)(int); // (-1) means to stop counting bars

    // start the run loop and fire 'callback' for every beginning of a bar
    void run(callback_t callback);

    State state = State::Wander;

private:
    const Config & _config;
    Layer _layers[16];
    Time _beat;
    Time _recorded;
    char _bars = 0; // # of recorded bars
};

} // midiate

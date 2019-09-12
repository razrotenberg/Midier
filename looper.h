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

    void play(int degree, char tag); // start playing a certain degree of the scale
    void stop(char tag); // stop playing every note labeled as 'tag'
    void run();

    // bars are in 1/4 time signature
    // we support the following note values:
    //   1) 1/16 (4 notes in a bar)
    //   2) 1/8 (2 notes in a bar)
    //   3) 1/8 triplet (3 notes in a bar)
    // therefore, we use a multiply of 12 for the number of subdivisions in a bar,
    // as it gives us the needed rhythmic precision
    // we use 96 in order to shorten the delay after every subdivision played
    //
    constexpr static auto Subdivisions = 96;

private:
    Config _config;
    Scale _scale;
    Layer _layers[16];
    char _subdivision = 0; // we can use 'char' as long as Looper::Subdivisions is less than 127
};

} // midiate

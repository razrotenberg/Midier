#pragma once

#include "triad.h"

namespace midiate
{

enum class Mode
{
    Ionian,
    Dorian,
    Phrygian,
    Lydian,
    Mixolydian,
    Aeolian,
    Locrian,
};

struct Scale
{
    Scale(Pitch root, Mode mode);
    
    // all supported scales are heptatonic

    Triad I;
    Triad II;
    Triad III;
    Triad IV;
    Triad V;
    Triad VI;
    Triad VII;
    Triad VIII;
};

} // midiate

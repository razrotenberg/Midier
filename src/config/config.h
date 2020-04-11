#pragma once

#include "../note/note.h"
#include "../octave/octave.h"
#include "../mode/mode.h"
#include "../rhythm/rhythm.h"

namespace midier
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

} // midier

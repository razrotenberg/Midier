#pragma once

#include "interval.h"
#include "note.h"
#include "sequence.h"

namespace midiate
{

typedef int Octave;

struct Pitch // representing a MIDI note number
{
    Pitch(); // value is (-1)
    Pitch(Note note, Accidental accidental, Octave octave);
    Pitch(char note);
    
    char note; // MIDI note number
};

Pitch operator+(Pitch pitch, Interval inteval);

template<>
inline bool Valid<Pitch>(const Pitch & pitch)
{
    return pitch.note != -1;
}

} // midiate

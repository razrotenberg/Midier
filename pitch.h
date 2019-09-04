#pragma once

#include "interval.h"
#include "note.h"

namespace midiate
{

typedef int Octave;

struct Pitch // representing a MIDI note number
{
    Pitch(); // a rest (pitch value is 0)
    Pitch(Note note, Accidental accidental, Octave octave);
    Pitch(int pitch);

    explicit operator int() const; // returns the respective MIDI note number

private:
    int _pitch;
};

Pitch operator+(Pitch pitch, Interval inteval);
bool operator==(Pitch lhs, Pitch rhs);
bool operator!=(Pitch lhs, Pitch rhs);

} // midiate

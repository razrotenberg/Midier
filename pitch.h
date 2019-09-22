#pragma once

#include "interval.h"
#include "note.h"
#include "sequence.h"

namespace midiate
{

typedef int Octave;

struct Pitch // representing a MIDI note number
{
    Pitch(); // value is 0
    Pitch(Note note, Accidental accidental, Octave octave);
    Pitch(int pitch);

    explicit operator int() const; // returns the respective MIDI note number

private:
    char _pitch; // we can use 'char' because the maximum value of a MIDI note number is 127
};

Pitch operator+(Pitch pitch, Interval inteval);
bool operator==(Pitch lhs, Pitch rhs);
bool operator!=(Pitch lhs, Pitch rhs);

template<>
struct Traits<Pitch>
{
    static void end(Pitch & pitch)
    {
        pitch = -1;
    }

    static bool ended(const Pitch & pitch)
    {
        return static_cast<int>(pitch) == -1;
    }
};

} // midiate

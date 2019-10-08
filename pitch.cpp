#include "pitch.h"

namespace midiate
{

Pitch::Pitch() : Pitch(-1)
{}

Pitch::Pitch(Note note, Accidental accidental, Octave octave) :
    note(24 + static_cast<int>(note) + static_cast<int>(accidental) + (12 * (octave - 1)))
{}

Pitch::Pitch(char note) :
    note(note)
{}

Pitch operator+(Pitch pitch, Interval interval)
{
    return Pitch(pitch.note + static_cast<char>(interval));
}

} // midiate

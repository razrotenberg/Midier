#include "pitch.h"

namespace midiate
{

Pitch::Pitch() : Pitch(0)
{}

Pitch::Pitch(Note note, Accidental accidental, Octave octave) :
    _pitch(21 + static_cast<int>(note) + static_cast<int>(accidental) + (12 * octave))
{}

Pitch::Pitch(int pitch) :
    _pitch(pitch)
{}

Pitch::operator int() const
{
    return _pitch;
}

Pitch operator+(Pitch pitch, Interval interval)
{
    return Pitch(static_cast<int>(pitch) + static_cast<int>(interval));
}

bool operator==(Pitch lhs, Pitch rhs)
{
    return static_cast<int>(lhs) == static_cast<int>(rhs);
}

bool operator!=(Pitch lhs, Pitch rhs)
{
    return !(lhs == rhs);
}

} // midiate

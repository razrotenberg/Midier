#include "pitch.h"

namespace midiate
{

Pitch::Pitch(Degree chord, Degree note) :
    _data((chord << 4) | (note & 0x0F))
{}

Degree Pitch::chord() const
{
    return (_data >> 4) & 0x0F;
}

Degree Pitch::note() const
{
    return _data & 0x0F;
}

} // midiate

#include "pitch.h"

namespace midiate
{

Pitch::Pitch(char chord, char note) :
    _data((chord << 4) | (note & 0x0F))
{}

char Pitch::chord() const
{
    return (_data >> 4) & 0x0F;
}

char Pitch::note() const
{
    return _data & 0x0F;
}

} // midiate

#pragma once

#include "sequence.h"

namespace midiate
{

struct Pitch
{
    Pitch() = default;
    Pitch(char chord, char note); // the chord degree in the scale and the note degree in the triad
    
    char chord() const; // the chord degree in the scale
    char note()  const; // the note degree in the triad

private:
    char _data = -1; // 4 msbs are the chord degree while the 4 lsbs are the note degree

    friend bool Valid<Pitch>(const Pitch &);
};

template<>
inline bool Valid<Pitch>(const Pitch & pitch)
{
    return pitch._data != -1;
}

} // midiate

#pragma once

namespace midiate
{

using Degree = char;

struct Pitch
{
    Pitch() = default;
    Pitch(Degree chord, Degree note); // the chord degree in the scale and the note degree in the triad
    
    Degree chord() const; // the chord degree in the scale
    Degree note()  const; // the note degree in the triad

private:
    char _data = -1; // 4 msbs are the chord degree while the 4 lsbs are the note degree
};

} // midiate

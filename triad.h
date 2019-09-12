#pragma once

#include "pitch.h"

namespace midiate
{

enum class Quality
{
    Major,
    Minor,
    Diminished,
    Augmented,
};

struct Triad
{
    Triad(Pitch root, Quality quality);

    Pitch I;
    Pitch III;
    Pitch V;

    const Pitch & degree(int i) const;
};

} // midiate

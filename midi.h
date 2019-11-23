#pragma once

#include "mode.h"
#include "note.h"
#include "octave.h"
#include "pitch.h"

namespace midiate
{
namespace midi
{

void play(Note root, Octave octave, Mode mode, const Pitch & pitch);

} // midi
} // midiate

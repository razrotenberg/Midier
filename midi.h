#pragma once

#include "degree.h"
#include "mode.h"
#include "note.h"
#include "octave.h"

namespace midiate
{
namespace midi
{

void play(Note root, Octave octave, Mode mode, Degree scale, Degree chord);

} // midi
} // midiate

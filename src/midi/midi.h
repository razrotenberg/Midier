#pragma once

#include "../degree/degree.h"
#include "../mode/mode.h"
#include "../note/note.h"
#include "../octave/octave.h"

namespace midiate
{
namespace midi
{

void play(Note root, Octave octave, Mode mode, Degree scale, Degree chord);

} // midi
} // midiate

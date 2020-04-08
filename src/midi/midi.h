#pragma once

#include "../degree/degree.h"
#include "../mode/mode.h"
#include "../note/note.h"
#include "../octave/octave.h"

namespace midier
{
namespace midi
{

void play(Note note);
void play(Note note, Octave octave);
void play(Note root, Octave octave, Mode mode, Degree scale, Degree chord);

} // midi
} // midier

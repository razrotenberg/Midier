#pragma once

namespace midiate
{

enum class Note
{
    // the values are the corresponding number of semitones from 'A' in
    // order to easily convert a note to its respective MIDI note number

    A = 0,
    B = 2,
    C = 3,
    D = 5,
    E = 7,
    F = 8,
    G = 10,
};

enum class Accidental
{
    // the values are the number of semitones (and direction)
    // the note should be modified by this accidental

    Flat    = -1,
    Natural =  0,
    Sharp   = +1,
};

} // midiate

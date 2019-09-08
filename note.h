#pragma once

namespace midiate
{

enum class Note
{
    // the values are the corresponding number of semitones from 'C' in
    // order to easily convert a note to its respective MIDI note number

    C = 0,
    D = 2,
    E = 4,
    F = 5,
    G = 7,
    A = 9,
    B = 11,
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

#pragma once

namespace midier
{

enum class Quality : char
{
    // in order to support both 7-th chords (4 notes) and triads (3 notes), we define
    // 3-note qualities as an alias to a 4-note quality with the same first 3 notes.
    // currently, qualities are used only to retrieve the interval of a chord degree
    // and there's no reason to be able to distinguish between 3-note qualities and
    // their matching 4-note qualities by having different enum values

    // seventh chords (4-note qualities)
    m7b5,   // half-diminished
    m7,     // minor seventh
    dom7,   // dominant seventh
    maj7,   // major seventh
    aug7,   // augmented seventh

    // triads (3-note qualities)
    dim = m7b5,     // diminished
    minor = m7,     // minor
    major = dom7,   // major
    aug = aug7,     // augmented
};

} // midier

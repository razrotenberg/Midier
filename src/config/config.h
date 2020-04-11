#pragma once

#include "../note/note.h"
#include "../octave/octave.h"
#include "../mode/mode.h"
#include "../rhythm/rhythm.h"

namespace midier
{

struct Config
{
    struct Style
    {
        // getters
        bool        looped()    const;
        unsigned    steps()     const;
        unsigned    perm()      const;

        // setters
        void looped (bool);
        void steps  (unsigned);
        void perm   (unsigned);

        // from lsb to msb:
        //   bits 0:9   - permutation - [0,6!=720)
        //   bits 10:12 - steps       - [3,6]
        //   bit  13    - looped      - [0,1]
        //
        // the two lsbs are not used
        private: short _data = 0b00110000000000UL; // looped 0; steps 3; perm 0;
    };

    Style style;

    // getters
    Note        note()          const;
    Accidental  accidental()    const;
    Octave      octave()        const;
    Mode        mode()          const;
    Rhythm      rhythm()        const;

    // setters
    void note       (Note);
    void accidental (Accidental);
    void octave     (Octave);
    void mode       (Mode);
    void rhythm     (Rhythm);

    // from lsb to msb:
    //   bits 0:3   - note          - [0,11]
    //   bits 4:5   - accidental    - [0,2]
    //   bits 6:8   - octave        - [1,7]
    //   bits 9:11  - mode          - [0,6]
    //   bits 12:15 - rhythm        - [0,10]
    private: short _data = 0b0111000011010000UL; // C3; Ionian; R7
};

} // midier

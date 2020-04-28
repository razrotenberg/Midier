#pragma once

#include "../note/note.h"
#include "../octave/octave.h"
#include "../mode/mode.h"
#include "../rhythm/rhythm.h"

namespace midier
{

namespace config
{

struct Packed
{
    // we want to be able to represent a configuraion in the minimum
    // possible amount of memory, as it is saved for every layer
    // therefore, we implement this packed version of it which stores
    // its data in a more compact way and not fully spanned and naive

    // creation
    Packed(); // default configuraion

    // argument specification
    Packed(Note);
    Packed(Note, Accidental);
    Packed(Note, Accidental, Octave);
    Packed(Note, Accidental, Octave, Mode);
    Packed(Note, Accidental, Octave, Mode, Rhythm);
    Packed(Note, Accidental, Octave, Mode, Rhythm, unsigned);
    Packed(Note, Accidental, Octave, Mode, Rhythm, unsigned, unsigned);
    Packed(Note, Accidental, Octave, Mode, Rhythm, unsigned, unsigned, bool);

    // getters
    Note        note()          const;
    Accidental  accidental()    const;
    Octave      octave()        const;
    Mode        mode()          const;
    Rhythm      rhythm()        const;
    unsigned    steps()         const;
    unsigned    perm()          const;
    bool        looped()        const;

    // in-place setters
    Packed & note       (Note);
    Packed & accidental (Accidental);
    Packed & octave     (Octave);
    Packed & mode       (Mode);
    Packed & rhythm     (Rhythm);
    Packed & steps      (unsigned);
    Packed & perm       (unsigned);
    Packed & looped     (bool);

    // copying setters
    Packed note       (Note)        const;
    Packed accidental (Accidental)  const;
    Packed octave     (Octave)      const;
    Packed mode       (Mode)        const;
    Packed rhythm     (Rhythm)      const;
    Packed steps      (unsigned)    const;
    Packed perm       (unsigned)    const;
    Packed looped     (bool)        const;

    // from lsb to msb:
    //   bits 0:3   - note          - [0,11]
    //   bits 4:5   - accidental    - [0,2]
    //   bits 6:8   - octave        - [1,7]
    //   bits 9:11  - mode          - [0,6]
    //   bits 12:15 - rhythm        - [0,10]
    //   bits 16:18 - steps         - [3,6]
    //   bits 19:28 - permutation   - [0,6!=720)
    //   bit  29    - looped        - [0,1]
    private: long _data;
};

struct Viewed
{
    // every layer may have its own configuraion, but we want to also
    // support sharing configuraion among layers.
    // therefore, every layer has to have a `Packed` member inside of it,
    // so it could hold a specific configuraion if detached from the common one.
    // in addition, the layer holds a view to the current configuration,
    // which could point to its own configuraion or to a common configuraion

    // creation
    Viewed(); // by default, statically configured
    Viewed(Viewed &&);
    Viewed & operator=(Viewed &&);

    Viewed(const Viewed &) = delete;
    Viewed& operator=(const Viewed &) = delete;

    Viewed(Packed *);

    // queries
    bool inner() const; // is pointing to the inner configuraion
    bool outer() const; // is pointing to another configuraion

    // getters
    const Packed & data();
    Packed * view();

    // access the viewed configuration
    Packed * operator->();

    // assign new configuration
    void operator=(const Packed & other);   // statically  - copying `other`
    void operator=(Packed * other);         // dynamically - pointing to `other`

private:
    Packed _data;
    Packed * _view;
};

} // config

using Config = config::Packed;

} // midier

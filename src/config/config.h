#pragma once

#include "../note/note.h"
#include "../octave/octave.h"
#include "../mode/mode.h"
#include "../rhythm/rhythm.h"

namespace midier
{

struct Config
{
    // a spanned representation of a layer configuraion

    Note        note        = Note::C;
    Accidental  accidental  = Accidental::Natural;
    Octave      octave      = 3;
    Mode        mode        = Mode::Ionian;
    Rhythm      rhythm      = Rhythm::_7;

    struct Style
    {
        unsigned    steps   = 3;
        unsigned    perm    = 0;
        bool        looped  = false;

        // creation
        Style() = default; // use all default values

        // argument specification
        Style(unsigned);
        Style(unsigned, unsigned);
        Style(unsigned, unsigned, bool);
    };

    Style style;

    // creation
    Config() = default; // use all default values

    // argument specification
    Config(Note);
    Config(Note, Accidental);
    Config(Note, Accidental, Octave);
    Config(Note, Accidental, Octave, Mode);
    Config(Note, Accidental, Octave, Mode, Rhythm);
    Config(Note, Accidental, Octave, Mode, Rhythm, Style);

    struct Packed
    {
        // we want to be able to represent `Config` in the minimum possible amount
        // of memory, as it is saved for every layer
        // therefore, we implement this packed version of `Config` which stores
        // its data in a more compact way and not fully spanned and naive

        // creation
        Packed();                           // default configuraion
        Packed(const Config &);             // from a spanned configuraion
        Packed & operator=(const Config &); // from a spanned configuraion

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

        struct Style
        {
            // getters
            unsigned    steps()     const;
            unsigned    perm()      const;
            bool        looped()    const;

            // setters
            void steps  (unsigned);
            void perm   (unsigned);
            void looped (bool);

            // from lsb to msb:
            //   bits 0:9   - permutation - [0,6!=720)
            //   bits 10:12 - steps       - [3,6]
            //   bit  13    - looped      - [0,1]
            //
            // the two lsbs are not used
            private: short _data;
        };

        Style style;

        // from lsb to msb:
        //   bits 0:3   - note          - [0,11]
        //   bits 4:5   - accidental    - [0,2]
        //   bits 6:8   - octave        - [1,7]
        //   bits 9:11  - mode          - [0,6]
        //   bits 12:15 - rhythm        - [0,10]
        private: short _data;
    };

    struct Viewed
    {
        // every layer may have its own configuraion, but we want to also
        // support sharing configuraion among layers.
        // therefore, every layer has to have a `Config` member inside of it,
        // so it could hold a specific configuraion if detached from the common one.
        // in addition, the layer holds a view to the current configuration,
        // which could point to its own configuraion or to a common configuraion

        // creation
        Viewed(); // by default, statically configured
        Viewed(Viewed &&);
        Viewed & operator=(Viewed &&);

        Viewed(const Viewed &) = delete;
        Viewed& operator=(const Viewed &) = delete;

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
};

} // midier

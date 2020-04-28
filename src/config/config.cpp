#include "config.h"

namespace midier
{
namespace config
{

namespace
{

inline long __set(long data, unsigned offset, long mask, long value)
{
    return (data & ~(mask << offset)) | ((value & mask) << offset);
}

inline long __get(long data, unsigned offset, long mask)
{
    return (data >> offset) & mask;
}

} //

Packed::Packed()
{
    note        (Note::C);
    accidental  (Accidental::Natural);
    octave      (3);
    mode        (Mode::Ionian);
    rhythm      (Rhythm::Triplet);
    steps       (3);
    perm        (0);
    looped      (false);
}

Packed::Packed(Note n)                                                                              : Packed()                      { note      (n);    }
Packed::Packed(Note n, Accidental a)                                                                : Packed(n)                     { accidental(a);    }
Packed::Packed(Note n, Accidental a, Octave o)                                                      : Packed(n, a)                  { octave    (o);    }
Packed::Packed(Note n, Accidental a, Octave o, Mode m)                                              : Packed(n, a, o)               { mode      (m);    }
Packed::Packed(Note n, Accidental a, Octave o, Mode m, Rhythm r)                                    : Packed(n, a, o, m)            { rhythm    (r);    }
Packed::Packed(Note n, Accidental a, Octave o, Mode m, Rhythm r, unsigned s)                        : Packed(n, a, o, m, r)         { steps     (s);    }
Packed::Packed(Note n, Accidental a, Octave o, Mode m, Rhythm r, unsigned s, unsigned p)            : Packed(n, a, o, m, r, s)      { perm      (p);    }
Packed::Packed(Note n, Accidental a, Octave o, Mode m, Rhythm r, unsigned s, unsigned p, bool l)    : Packed(n, a, o, m, r, s, p)   { looped    (l);    }

Note Packed::note() const
{
    return (Note)__get(_data, 0, 0b1111);
}

Accidental Packed::accidental() const
{
    return (Accidental)(__get(_data, 4, 0b11) - 1);
}

Octave Packed::octave() const
{
    return (Octave)__get(_data, 6, 0b111);
}

Mode Packed::mode() const
{
    return (Mode)__get(_data, 9, 0b111);
}

Rhythm Packed::rhythm() const
{
    return (Rhythm)__get(_data, 12, 0b1111);
}

unsigned Packed::steps() const
{
    return __get(_data, 16, 0b111);
}

unsigned Packed::perm() const
{
    return __get(_data, 19, 0b1111111111);
}

bool Packed::looped() const
{
    return __get(_data, 29, 0b1);
}

Packed & Packed::note(Note value)
{
    _data = __set(_data, 0, 0b1111, (long)value);
    return *this;
}

Packed & Packed::accidental(Accidental value)
{
    _data = __set(_data, 4, 0b11, (long)value + 1);
    return *this;
}

Packed & Packed::octave(Octave value)
{
    _data = __set(_data, 6, 0b111, value);
    return *this;
}

Packed & Packed::mode(Mode value)
{
    _data = __set(_data, 9, 0b111, (long)value);
    return *this;
}

Packed & Packed::rhythm(Rhythm value)
{
    _data = __set(_data, 12, 0b1111, (long)value);
    return *this;
}

Packed & Packed::steps(unsigned value)
{
    _data = __set(_data, 16, 0b111, value);
    return *this;
}

Packed & Packed::perm(unsigned value)
{
    _data = __set(_data, 19, 0b1111111111, value);
    return *this;
}

Packed & Packed::looped(bool value)
{
    _data = __set(_data, 29, 0b1, (long)value);
    return *this;
}

Packed Packed::note(Note value) const
{
    return Packed(*this).note(value);
}

Packed Packed::accidental(Accidental value) const
{
    return Packed(*this).accidental(value);
}

Packed Packed::octave(Octave value) const
{
    return Packed(*this).octave(value);
}

Packed Packed::mode(Mode value) const
{
    return Packed(*this).mode(value);
}

Packed Packed::rhythm(Rhythm value) const
{
    return Packed(*this).rhythm(value);
}

Packed Packed::steps(unsigned value) const
{
    return Packed(*this).steps(value);
}

Packed Packed::perm(unsigned value) const
{
    return Packed(*this).perm(value);
}

Packed Packed::looped(bool value) const
{
    return Packed(*this).looped(value);
}

Viewed::Viewed() :
    _data(),
    _view(&_data)
{}

Viewed::Viewed(Viewed && config)
{
    *this = (Viewed &&)config;
}

Viewed & Viewed::operator=(Viewed && other)
{
    if (other.inner())
    {
        *this = other.data();
    }
    else
    {
        *this = other.view();
    }

    return *this;
}

Viewed::Viewed(Packed * config) : Viewed()
{
    *this = config;
}

bool Viewed::inner() const
{
    return _view == &_data;
}

bool Viewed::outer() const
{
    return !inner();
}

const Packed & Viewed::data()
{
    return _data;
}

Packed * Viewed::view()
{
    return _view;
}

Packed * Viewed::operator->()
{
    return _view;
}

void Viewed::operator=(const Packed & other)
{
    _view = &(_data = other);
}

void Viewed::operator=(Packed * other)
{
    _view = other;
};

} // config
} // midier

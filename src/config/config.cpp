#include "config.h"

namespace midier
{

namespace
{

inline short __set(short data, unsigned offset, short mask, short value)
{
    return (data & ~(mask << offset)) | ((value & mask) << offset);
}

inline short __get(short data, unsigned offset, short mask)
{
    return (data >> offset) & mask;
}

} //

bool Config::Style::looped() const
{
    return __get(_data, 13, 0b1);
}

unsigned Config::Style::steps() const
{
    return __get(_data, 10, 0b111);
}

unsigned Config::Style::perm() const
{
    return __get(_data, 0, 0b1111111111);
}

void Config::Style::looped(bool value)
{
    _data = __set(_data, 13, 0b1, (short)value);
}

void Config::Style::steps(unsigned value)
{
    _data = __set(_data, 10, 0b111, value);
}

void Config::Style::perm(unsigned value)
{
    _data = __set(_data, 0, 0b1111111111, value);
}

Note Config::note() const
{
    return (Note)__get(_data, 0, 0b1111);
}

Accidental Config::accidental() const
{
    return (Accidental)(__get(_data, 4, 0b11) - 1);
}

Octave Config::octave() const
{
    return (Octave)__get(_data, 6, 0b111);
}

Mode Config::mode() const
{
    return (Mode)__get(_data, 9, 0b111);
}

Rhythm Config::rhythm() const
{
    return (Rhythm)__get(_data, 12, 0b1111);
}

void Config::note(Note value)
{
    _data = __set(_data, 0, 0b1111, (short)value);
}

void Config::accidental(Accidental value)
{
    _data = __set(_data, 4, 0b11, (short)value + 1);
}

void Config::octave(Octave value)
{
    _data = __set(_data, 6, 0b111, value);
}

void Config::mode(Mode value)
{
    _data = __set(_data, 9, 0b111, (short)value);
}

void Config::rhythm(Rhythm value)
{
    _data = __set(_data, 12, 0b1111, (short)value);
}

} // midier

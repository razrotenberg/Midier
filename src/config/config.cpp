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

Config::Packed::Packed() : Packed(Config()) // use default config
{}

Config::Packed::Packed(const Config & unpacked)
{
    *this = unpacked;
}

Config::Packed & Config::Packed::operator=(const Config & unpacked)
{
    note        (unpacked.note);
    accidental  (unpacked.accidental);
    octave      (unpacked.octave);
    mode        (unpacked.mode);
    rhythm      (unpacked.rhythm);

    style.looped(unpacked.style.looped);
    style.steps (unpacked.style.steps);
    style.perm  (unpacked.style.perm);
}

Note Config::Packed::note() const
{
    return (Note)__get(_data, 0, 0b1111);
}

Accidental Config::Packed::accidental() const
{
    return (Accidental)(__get(_data, 4, 0b11) - 1);
}

Octave Config::Packed::octave() const
{
    return (Octave)__get(_data, 6, 0b111);
}

Mode Config::Packed::mode() const
{
    return (Mode)__get(_data, 9, 0b111);
}

Rhythm Config::Packed::rhythm() const
{
    return (Rhythm)__get(_data, 12, 0b1111);
}

void Config::Packed::note(Note value)
{
    _data = __set(_data, 0, 0b1111, (short)value);
}

void Config::Packed::accidental(Accidental value)
{
    _data = __set(_data, 4, 0b11, (short)value + 1);
}

void Config::Packed::octave(Octave value)
{
    _data = __set(_data, 6, 0b111, value);
}

void Config::Packed::mode(Mode value)
{
    _data = __set(_data, 9, 0b111, (short)value);
}

void Config::Packed::rhythm(Rhythm value)
{
    _data = __set(_data, 12, 0b1111, (short)value);
}

bool Config::Packed::Style::looped() const
{
    return __get(_data, 13, 0b1);
}

unsigned Config::Packed::Style::steps() const
{
    return __get(_data, 10, 0b111);
}

unsigned Config::Packed::Style::perm() const
{
    return __get(_data, 0, 0b1111111111);
}

void Config::Packed::Style::looped(bool value)
{
    _data = __set(_data, 13, 0b1, (short)value);
}

void Config::Packed::Style::steps(unsigned value)
{
    _data = __set(_data, 10, 0b111, value);
}

void Config::Packed::Style::perm(unsigned value)
{
    _data = __set(_data, 0, 0b1111111111, value);
}

Config::Viewed::Viewed() :
    _data(),
    _view(&_data)
{}

Config::Viewed::Viewed(Config::Viewed && config)
{
    *this = (Config::Viewed &&)config;
}

Config::Viewed & Config::Viewed::operator=(Config::Viewed && other)
{
    if (other._view == &other._data) // static
    {
        *this = other.data();
    }
    else // dynamic
    {
        *this = other.view();
    }

    return *this;
}

const Config::Packed & Config::Viewed::data()
{
    return _data;
}

Config::Packed * Config::Viewed::view()
{
    return _view;
}

Config::Packed * Config::Viewed::operator->()
{
    return _view;
}

void Config::Viewed::operator=(const Config::Packed & other)
{
    _view = &(_data = other);
}

void Config::Viewed::operator=(Config::Packed * other)
{
    _view = other;
};

} // midier

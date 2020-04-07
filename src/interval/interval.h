#pragma once

namespace midier
{

enum class Interval : char
{
    // the values are the corresponding numbers of semitones

    P1 = 0,
    m2 = 1,
    M2 = 2,
    m3 = 3,
    M3 = 4,
    P4 = 5,
    A4 = 6,
    d5 = 6,
    P5 = 7,
    A5 = 8,
    m6 = 8,
    M6 = 9,
    m7 = 10,
    M7 = 11,
    P8 = 12,
};

inline Interval operator+(Interval lhs, Interval rhs)
{
    return (Interval)((char)lhs + (char)rhs);
}

} // midier

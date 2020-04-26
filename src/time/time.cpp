#include "time.h"

namespace midier
{

Time Time::now = { .bar = 0, .subdivision = 0 };

namespace
{

inline char mod(char i, char n) // positive mod operator
{
    return static_cast<char>(((i % n) + n) % n);
}

} //

Time::Time(char bar, char subdivision) :
    bar(bar),
    subdivision(subdivision)
{}

void Time::operator++()
{
    subdivision = (subdivision + 1) % Time::Subdivisions;

    if (subdivision == 0)
    {
        bar = (bar + 1) % Time::Bars;
    }
}

Time::Duration::Duration(float bars)
{
    const unsigned integer = (unsigned)bars;
    const float fractional = (bars - (long)bars);

    this->bars = integer;
    this->subdivisions = fractional * Time::Subdivisions;
}

unsigned Time::Duration::total() const
{
    return (unsigned)bars * Time::Subdivisions + (unsigned)subdivisions;
}

bool operator==(const Time & lhs, const Time & rhs)
{
    return lhs.bar == rhs.bar && lhs.subdivision == rhs.subdivision;
}

bool operator!=(const Time & lhs, const Time & rhs)
{
    return !(lhs == rhs);
}

Time::Difference operator-(const Time & to, const Time & from)
{
    char bars = mod(to.bar - from.bar, Time::Bars);

    if (to.subdivision < from.subdivision)
    {
        --bars;
    }

    return
    {
        .bars = mod(bars, Time::Bars),
        .subdivisions = mod(to.subdivision - from.subdivision, Time::Subdivisions)
    };
}

} // midier

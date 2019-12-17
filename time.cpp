#include "time.h"

namespace midiate
{

namespace
{

inline char mod(char i, char n) // positive mod operator
{
    return static_cast<char>(((i % n) + n) % n);
}

} //

void Time::operator++()
{
    subdivision = (subdivision + 1) % Time::Subdivisions;

    if (subdivision == 0)
    {
        bar = (bar + 1) % Time::Bars;
    }
}

bool Time::ordered(const Time & first, const Time & second, const Time & third)
{
    return \
        (first <= second && second <= third)    ||
        (third <= first && first <= second)     ||
        (second <= third && third <= first);
}

bool operator==(const Time & lhs, const Time & rhs)
{
    return lhs.bar == rhs.bar && lhs.subdivision == rhs.subdivision;
}

bool operator!=(const Time & lhs, const Time & rhs)
{
    return !(lhs == rhs);
}

bool operator<=(const Time & lhs, const Time & rhs)
{
    if (lhs.bar < rhs.bar)
    {
        return true;
    }
    else if (lhs.bar > rhs.bar)
    {
        return false;
    }

    return lhs.subdivision <= rhs.subdivision;
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

} // midiate

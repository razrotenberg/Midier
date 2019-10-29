#include "beat.h"

namespace midiate
{

Beat & Beat::operator++()
{
    subdivision = (subdivision + 1) % Beat::Subdivisions;

    if (bar != -1 && subdivision == start)
    {
        ++bar;
    }

    return *this;
}

bool operator==(const Time & lhs, const Time & rhs)
{
    return lhs.bar == rhs.bar && lhs.subdivision == rhs.subdivision;
}

} // midiate

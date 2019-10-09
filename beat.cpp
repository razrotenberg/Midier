#include "beat.h"

namespace midiate
{

Beat & Beat::operator++()
{
    subdivision = (subdivision + 1) % Beat::Subdivisions;

    if (bar != -1 && subdivision == 0)
    {
        ++bar;
    }

    return *this;
}

bool operator==(const Beat & lhs, const Beat & rhs)
{
    return lhs.bar == rhs.bar && lhs.subdivision == rhs.subdivision;
}

} // midiate

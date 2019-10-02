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

} // midiate

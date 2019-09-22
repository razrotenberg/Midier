#include "beat.h"

namespace midiate
{

Beat & Beat::operator++()
{
    subdivision = (subdivision + 1) % Beat::Subdivisions;

    if (bar.index != -1 && subdivision == 0)
    {
        ++bar.index;
    }

    return *this;
}

Beat operator+(const Beat & beat, int subdivisions)
{
    const auto target = beat.subdivision + subdivisions;

    auto result = beat;

    if (result.bar.index != -1)
    {
        result.bar.index += (target >= Beat::Subdivisions);
    }

    result.subdivision = target % Beat::Subdivisions;

    return result;
}

} // midiate

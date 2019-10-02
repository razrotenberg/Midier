#pragma once

namespace midiate
{

template <typename T>
struct Marker // a per-type implementation of marking an element as an end indicator
{
    static void end(T&);
    static bool ended(const T&);
};

} // midiate

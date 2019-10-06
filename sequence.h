#pragma once

#include "marker.h"

namespace midiate
{

template <typename T, char S>
struct Sequence
{
    Sequence()
    {
        revoke();
    }

    T & operator[] (int i)
    {
        return _elements[i];
    }

    T & operator*()
    {
        return this->operator[](_pos);
    }

    void operator++()
    {
        _pos = (_pos + 1) % S;

        if (Marker<T>::ended(this->operator*()))
        {
            _pos = 0;
        }
    }

    using cond_t = bool(*)(const T &);

    void revoke(cond_t cond = nullptr)
    {
        for (auto & element : _elements)
        {
            if (cond != nullptr && cond(element) == false)
            {
                continue;
            }

            Marker<T>::end(element);
        }

        if (Marker<T>::ended(this->operator*()))
        {
            _pos = 0;
        }
    }

private:
    T _elements[S];
    char _pos = 0; // we can use 'char' as long as 'S' is a char as well
};

} // midiate

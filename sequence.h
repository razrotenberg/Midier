#pragma once

namespace midiate
{

template <typename T, char S>
struct Sequence
{
    Sequence()
    {
        for (auto & element : _elements)
        {
            element = -1;
        }
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

        if (static_cast<int>(this->operator*()) == -1)
        {
            _pos = 0;
        }
    }

private:
    T _elements[S];
    char _pos = 0; // we can use 'char' as long as 'S' is a char as well
};

} // midiate

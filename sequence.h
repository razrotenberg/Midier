#pragma once

namespace midiate
{

template <typename T>
bool Valid(const T&);

template <typename T, char S>
struct Sequence
{
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

        if (Valid<T>(this->operator*()) == false)
        {
            _pos = 0;
        }
    }

private:
    T _elements[S];
    char _pos = 0; // we can use 'char' as long as 'S' is a char as well
};

} // midiate

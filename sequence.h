#pragma once

namespace midiate
{

template <typename T>
struct Traits // a per-type implementation of marking an element as an end indicator
{
    static void end(T&);
    static bool ended(const T&);
};

template <typename T, char S>
struct Sequence
{
    Sequence()
    {
        for (auto & element : _elements)
        {
            Traits<T>::end(element);
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

        if (Traits<T>::ended(this->operator*()))
        {
            _pos = 0;
        }
    }

private:
    T _elements[S];
    char _pos = 0; // we can use 'char' as long as 'S' is a char as well
};

} // midiate

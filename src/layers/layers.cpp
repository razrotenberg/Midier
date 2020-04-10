#include "layers.h"

namespace midier
{

Layers::Iterator::Iterator(Layers & layers, unsigned index) :
    _layers(layers),
    _index(index)
{}

Layer & Layers::Iterator::operator*()
{
    return _layers[_index];
}

void Layers::Iterator::operator++()
{
    ++_index;
}

bool Layers::Iterator::operator!=(const Iterator & other)
{
    return _index != other._index;
}

Layers::Layers(Layer layers[], unsigned count) :
    _layers(layers),
    _count(count)
{}

unsigned Layers::count() const
{
    return _count;
}

unsigned Layers::used() const
{
    unsigned count = 0;

    for (auto & layer : (Layers &)*this)
    {
        if (layer.tag != -1)
        {
            ++count;
        }
    }

    return count;
}

bool Layers::idle() const
{
    return used() == 0;
}

Layer * Layers::find(char tag)
{
    for (auto & layer : *this)
    {
        if (layer.tag == tag)
        {
            return &layer;
        }
    }

    return nullptr;
}

Layer & Layers::operator[](unsigned index)
{
    return _layers[index];
}

Layers::Iterator Layers::begin()
{
    return Iterator(*this, 0);
}

Layers::Iterator Layers::end()
{
    return Iterator(*this, count());
}

void Layers::eval(void(*callback)(Layer &))
{
    for (auto & layer : *this)
    {
        if (layer.tag == -1)
        {
            continue;
        }

        callback(layer);
    }
}

bool Layers::any(bool(*predicate)(const Layer &))
{
    for (const auto & layer : *this)
    {
        if (layer.tag == -1)
        {
            continue;
        }

        if (predicate(layer))
        {
            return true;
        }
    }

    return false;
}

bool Layers::none(bool(*predicate)(const Layer &))
{
    return !any(predicate);
}

void Layers::record()
{
    eval([](Layer & layer) { layer.record(); });
}

void Layers::click()
{
    eval([](Layer & layer) { layer.click(); });
}

void Layers::revoke()
{
    eval([](Layer & layer) { layer.revoke(); });
}

void Layers::play()
{
    eval([](Layer & layer) { layer.play(); });
}

} // midier

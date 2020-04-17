#pragma once

#include "../layer/layer.h"

namespace midier
{

struct ILayers
{
    struct Iterator
    {
        Iterator(ILayers & layers, unsigned index);

        Layer & operator *();
        void    operator++();
        bool    operator!=(const Iterator & other);

    private:
        ILayers & _layers;
        unsigned _index;
    };

    ILayers(Layer layers[], unsigned count);

    // queries
    unsigned count() const; // how many there are in total
    unsigned running() const; // how many layers are running
    bool idle() const; // if no layers are running

    // getters
    Layer & operator[](unsigned index);

    // iterate
    Iterator begin();
    Iterator end();

    // generic lambda evaluation methods
    // they iterate and evaluate on used layers only
    void eval(void(*callback)(Layer &));
    bool all(bool(*predicate)(const Layer &));
    bool any(bool(*predicate)(const Layer &));
    bool none(bool(*predicate)(const Layer &));

    // aggregated `Layer` methods
    // they are evaluated on all used layers
    void record();
    void click();
    void revoke();

private:
    Layer * _layers;
    unsigned _count;
};

// same as `ILayers` but hold the data as well
// clients should use this class to declare their layers
template <unsigned N>
struct Layers : ILayers
{
    Layers() : ILayers(_layers, N)
    {}

    private: Layer _layers[N];
};

} // midier

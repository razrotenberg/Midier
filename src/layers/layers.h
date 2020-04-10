#pragma once

#include "../layer/layer.h"

namespace midier
{

struct Layers
{
    struct Iterator
    {
        Iterator(Layers & layers, unsigned index);

        Layer & operator *();
        void    operator++();
        bool    operator!=(const Iterator & other);

    private:
        Layers & _layers;
        unsigned _index;
    };

    Layers(Layer layers[], unsigned count);

    // queries
    unsigned count() const;
    unsigned used() const; // how many layers are used
    bool idle() const; // if no layers are used

    // getters
    Layer * find(char tag); // returns `nullptr` if there's no layer with this tag
    Layer & operator[](unsigned index);

    // iterate
    Iterator begin();
    Iterator end();

    // generic lambda evaluation methods
    // they iterate and evaluate on used layers only
    void eval(void(*callback)(Layer &));
    bool any(bool(*predicate)(const Layer &));
    bool none(bool(*predicate)(const Layer &));

    // aggregated `Layer` methods
    // they are evaluated on all used layers
    void record();
    void click();
    void revoke();
    void play();

private:
    Layer * _layers;
    unsigned _count;
};

} // midier

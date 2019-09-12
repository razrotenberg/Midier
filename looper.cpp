#include "looper.h"
#include "midi.h"

#include <Arduino.h>

namespace midiate
{

Looper::Looper(const Config & config) :
    _config(config),
    _scale(
        Pitch(config.note, config.accidental, config.octave),
        config.mode
    )
{}

void Looper::config(const Config & config)
{
    if (_config.note != config.note             ||
        _config.accidental != config.accidental ||
        _config.octave != config.octave         ||
        _config.mode != config.mode)
    {
        // regenerate '_scale' only if needed

        _scale = Scale(
            Pitch(config.note, config.accidental, config.octave),
            config.mode
        );
    }

    _config = config;
}

void Looper::play(int degree, char tag)
{
    for (auto & layer : _layers)
    {
        if (layer.tag != -1)
        {
            continue; // this layer is used
        }

        layer = Layer(
            _scale.degree(degree),
            _config.style,
            _config.rhythm,
            _subdivision + 1,
            tag
        );

        break;
    }
}

void Looper::stop(char tag)
{
    for (auto & layer : _layers)
    {
        if (layer.tag != tag)
        {
            continue;
        }

        layer.tag = -1; // mark as unused
    }
}

void Looper::run()
{
    while (true)
    {
        for (auto & layer : _layers)
        {
            if (layer.tag == -1)
            {
                continue; // unused layer
            }

            layer.play(_subdivision);
        }
        
        delay(60.f / static_cast<float>(_config.bpm) * 1000.f / static_cast<float>(Looper::Subdivisions));

        _subdivision = (_subdivision + 1) % Looper::Subdivisions;
    }
}

} // midiate

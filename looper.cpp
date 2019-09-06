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

void Looper::play(int degree, int tag)
{
    const auto & triad = _scale.degree(degree);

    Pitch pitches[] = { triad.I, triad.III, triad.V };

    for (int i = 0; i < sizeof(pitches) / sizeof(Pitch); ++i)
    {
        for (auto & description : _descriptions)
        {
            if (description.tag != -1)
            {
                continue; // this description is used
            }

            description.pitch = pitches[i];
            description.subdivision = (_pos + 1 + (Looper::Subdivisions / 3 * i)) % Looper::Subdivisions; // currently we support only 1/8 note triplets
            description.tag = tag;
            break;
        }
    }
}

void Looper::stop(int tag)
{
    for (auto & description : _descriptions)
    {
        if (description.tag != tag)
        {
            continue;
        }

        description.subdivision = -1;
        description.tag = -1;
    }
}

void Looper::run()
{
    while (true)
    {
        if (_pos == Looper::Subdivisions)
        {
            _pos = 0;
        }

        for (const auto & description : _descriptions)
        {
            if (description.subdivision != _pos)
            {
                continue;
            }

            midi::play(description.pitch);
        }
        
        delay(60.f / static_cast<float>(_config.bpm) * 1000.f / static_cast<float>(Looper::Subdivisions));

        ++_pos;
    }
}

} // midiate

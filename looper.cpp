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

char Looper::start(int degree)
{
    for (char i = 0; i < sizeof(_layers) / sizeof(Layer); ++i)
    {
        auto & layer = _layers[i];

        if (layer.tag != -1)
        {
            continue; // this layer is used
        }

        // we use random numbers for the tags mainly to support revoking layers 
        // (when starting to record). we want to make sure stopping a revoked
        // layer has no effect, and avoid the reuse of tags of revoked layers
        const auto tag = random(0, 128); // [0, 127]

        layer = Layer(
            _scale.degree(degree),
            _config.style,
            _config.rhythm,
            _beat.subdivision + 1, // it's ok to overlflow as we correct this in Layer::Layer()
            tag
        );

        if (_state == State::Record || _state == State::Overlay)
        {
            layer.record();
        }

        return tag;
    }

    return -1; // there's no place for a new layer
}

void Looper::stop(char tag)
{
    for (auto & layer : _layers)
    {
        if (layer.tag != tag)
        {
            continue;
        }

        if (_state == State::Record)
        {
            layer.playback();
        }
        else
        {
            layer.tag = -1; // mark as unused and hence stop it from being played
        }
    }
}

void Looper::run(callback_t callback)
{
    auto previous = _state;

    while (true)
    {
        noInterrupts(); // '_state' may be set by record() which is called via interrupts
        if (_state == State::Record)
        {
            if (previous == State::Wander) // just starting to record
            {
                _beat.subdivision = 0; // reset the beat
                _beat.bar = 0; // start counting bars

                for (auto & layer : _layers)
                {
                    // we revoke all (active) layers and mark them as unused because we have
                    // just reset the beat, so any moment of any layer is actually invalid anymore
                    layer.tag = -1;
                }
            }

            if (_beat.subdivision == 0 && _bars < sizeof(Moment::bars) * 8) // maximum # of bars
            {
                ++_bars; // increase the # of recorded bars when entering a new bar (not when exiting one)
            }
        }

        previous = _state;
        interrupts();

        if (_beat.subdivision == 0 && callback != nullptr)
        {
            callback(_beat.bar);
        }

        for (auto & layer : _layers)
        {
            if (layer.tag == -1)
            {
                continue; // unused layer
            }

            layer.play(_beat);
        }
        
        delay(60.f / static_cast<float>(_config.bpm) * 1000.f / static_cast<float>(Beat::Subdivisions));

        ++_beat;

        if (_beat.bar == _bars) // relevant only for 'record' mode ('_beat.bar' will be (-1) in other cases)
        {
            _beat.bar = 0;
        }
    }
}

void Looper::record()
{
    _state = State::Record;
}

void Looper::playback()
{
    _state = State::Playback;
}

void Looper::overlay()
{
    _state = State::Overlay;
}

Looper::State Looper::state() const
{
    return _state;
}

} // midiate

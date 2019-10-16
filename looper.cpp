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
    ),
    _layers(),
    _beat(),
    _state(State::Wander),
    _bars(0)
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

        // we use random tags to support revoking layers when starting to record
        // we want to make sure stopping a revoked layer has no side effects such
        // as reusing tags of revoked layers
        const auto tag = random(0, 128); // [0, 127]

        layer = Layer(
            _scale.degree(degree),
            _config.style,
            _config.rhythm,
            _beat,
            tag
        );

        if (_state == State::Record || _state == State::Overlay)
        {
            layer.state = Layer::State::Record;
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

        if (layer.state == Layer::State::Wander)
        {
            layer.tag = -1; // mark as unused
        }
        else if (layer.state == Layer::State::Record)
        {
            layer.state = Layer::State::Playback;
        }

        // do nothing if the layer is already in playback mode (let it keep playbacking)
        // this could happen when the button is kept being pressed even after the entire
        // recorded loop (32 bars) is over
    }
}

void Looper::run(callback_t callback)
{
    auto previous = _state;

    while (true)
    {
        // we disable interrupts because we don't want any user actions to interfere the main logic
        // both '_state' and the '_layers' may be modified, and '_beat' may be accessed via interrupts
        noInterrupts();

        if (_state == State::Record && previous == State::Wander) // just starting to record
        {
            _beat.bar = 0; // start counting bars
            _beat.subdivision = 0; // reset the beat

            for (auto & layer : _layers) // revoke all layers
            {
                // we revoke all layers because we have just reset the beat
                // and any moment of any layer is in fact invalid anymore
                layer.tag = -1;
            }
        }

        if (_beat.subdivision == 0)
        {
            if (_state == State::Record && _bars < sizeof(Moment::bars) * 8) // maximum # of bars
            {
                ++_bars; // increase the # of recorded bars when (recording and) entering a new bar
            }

            if (_state == State::Record || _state == State::Playback || _state == State::Overlay)
            {
                if (_beat.bar == _bars) // just passed the # of recorded bars
                {
                    _beat.bar = 0;
                }
            
                if (callback != nullptr)
                {
                    callback(_beat.bar);
                }
            }
        }

        for (auto & layer : _layers)
        {
            if (layer.tag == -1)
            {
                continue; // unused layer
            }

            layer.play(_beat);
        }

        ++_beat;

        for (auto & layer : _layers)
        {
            if (layer.tag == -1)
            {
                continue; // unused layer
            }

            if (_beat == layer.start) // the layer has completed a full loop
            {
                if (layer.state == Layer::State::Record)
                {
                    layer.state = Layer::State::Playback; // stop recording the layer and start playing it back
                }

                if (layer.state == Layer::State::Playback) // not 'else if'
                {
                    // we reset the layer's pitches and moments because we want the layer to playback exactly what recorded
                    layer.reset();
                }
            }
        }

        previous = _state;
        
        // enable interrupts as we are done with the main logic and no need for locks anymore
        interrupts();

        delay(60.f / static_cast<float>(_config.bpm) * 1000.f / static_cast<float>(Beat::Subdivisions));
    }
}

} // midiate

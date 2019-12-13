#include "looper.h"
#include "midi.h"

#include <Arduino.h>

namespace midiate
{

Looper::Looper(const Config & config) :
    state(State::Wander),
    _config(config),
    _layers(),
    _beat(),
    _bars(0)
{}

char Looper::start(char degree)
{
    for (char i = 0; i < sizeof(_layers) / sizeof(Layer); ++i)
    {
        auto & layer = _layers[i];

        if (layer.tag != -1)
        {
            continue; // this layer is used
        }

        layer = Layer(i, degree, _beat);

        if (state == State::Record || state == State::Overlay)
        {
            layer.record.start = _beat;
            layer.state = Layer::State::Record;
        }

        return i;
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
            layer.record.end = _beat;
            layer.state = Layer::State::Playback;
        }

        // do nothing if the layer is already in playback mode (let it keep playbacking)
        // this could happen when the button is kept being pressed even after the entire
        // recorded loop (32 bars) is over
    }
}

void Looper::undo()
{
    if (state == State::Wander)
    {
        return; // nothing to do when wandering
    }

    // we want to stop the last recorded (or being recorded) layer
    // we cannot tell for sure which layer was the last one to be recorded,
    // so we assume it is the layer with the highest tag (and the highest index)

    for (unsigned i = sizeof(_layers) / sizeof(Layer); i > 0; --i)
    {
        auto & layer = _layers[i - 1];

        if (layer.tag == -1)
        {
            continue;
        }

        if (layer.state == Layer::State::Record || layer.state == Layer::State::Playback)
        {
            layer.tag = -1;
            break;
        }
    }

    // now check if there is no more recorded layers

    for (const auto & layer : _layers)
    {
        if (layer.tag == -1)
        {
            continue;
        }

        if (layer.state == Layer::State::Record || layer.state == Layer::State::Playback)
        {
            return;
        }
    }

    // seems like there are no such layers, go back to wander mode
    state = State::Wander;
}

void Looper::run(callback_t callback)
{
    auto previous = state;

    while (true)
    {
        // we disable interrupts because we don't want any user actions to interfere the main logic
        // both 'state' and the '_layers' may be modified, and '_beat' may be accessed via interrupts
        noInterrupts();

        if (state == State::Record && previous == State::Wander) // just starting to record
        {
            _recorded = _beat;

            for (auto & layer : _layers) // start recording all layers
            {
                if (layer.tag == -1)
                {
                    continue; // this layer is not used
                }

                layer.record.start = _beat;
                layer.state = Layer::State::Record;
            }
        }
        else if (state == State::Wander && previous != State::Wander)
        {
            _bars = 0; // reset the # of recorded bars

            for (auto & layer : _layers) // revoke all layers
            {
                layer.tag = -1;
            }
            
            callback(-1); // clear the bar
        }

        if (state == State::Record || state == State::Playback || state == State::Overlay)
        {
            const auto difference = _beat - _recorded;

            if (difference.subdivisions == 0) // another whole bar has passed since we started recording
            {
                if (state == State::Record && _bars < Time::Bars) // still recording and haven't reached the max # of bars yet
                {
                    ++_bars; // increase the # of recorded bars when (recording and) entering a new bar
                }

                if (difference.bars == _bars) // just passed the # of recorded bars
                {
                    _beat.bar = _recorded.bar;

                    // mark the record end for all the layers that was played before recording
                    // i.e. timeline: [play ... start record ... end record]
                    //
                    for (auto & layer : _layers)
                    {
                        if (layer.tag == -1) { continue; }

                        if (layer.state == Layer::State::Record && _beat == layer.record.start)
                        {
                            layer.record.end = _beat;
                            layer.state = Layer::State::Playback;
                        }
                    }
                }

                callback((_beat - _recorded).bars);
            }
        }

        for (auto & layer : _layers)
        {
            if (layer.tag == -1)
            {
                continue; // unused layer
            }

            Pitch pitch;
            if (layer.play(_beat, _config.style, _config.rhythm, /* out */ pitch))
            {
                midi::play(
                    _config.note + _config.accidental,
                    _config.octave,
                    _config.mode,
                    pitch
                );
            }
        }

        ++_beat;

        for (auto & layer : _layers)
        {
            if (layer.tag == -1)
            {
                continue; // unused layer
            }

            // mark the record end for all the layers that have completed a full loop
            // i.e. timeline: [start record ... play ... end record]
            //
            if (layer.state == Layer::State::Record && _beat == layer.record.start)
            {
                layer.record.end = _beat;
                layer.state = Layer::State::Playback;
            }

            if ((_beat - layer.start).subdivisions == 0)
            {
                layer.counter = (layer.counter + 1) % Layer::Period; // the layer was played another bar
            }
        }

        previous = state;
        
        // enable interrupts as we are done with the main logic and no need for locks anymore
        interrupts();

        delay(60.f / static_cast<float>(_config.bpm) * 1000.f / static_cast<float>(Time::Subdivisions));
    }
}

} // midiate

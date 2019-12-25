#include "looper.h"

#include "debug.h"
#include "midi.h"

#include <Arduino.h>

namespace midiate
{

Looper::Looper(const Config & config) :
    _config(config)
{}

char Looper::start(Degree degree)
{
    for (char i = 0; i < sizeof(_layers) / sizeof(Layer); ++i)
    {
        auto & layer = _layers[i];

        if (layer.tag != -1)
        {
            continue; // this layer is used
        }

        layer = Layer(i, degree, _beat);

        TRACE_6("Starting layer ", layer, " of scale degree ", degree, " at beat ", _beat);

        if (state == State::Record || state == State::Overlay)
        {
            layer.record(_beat);
        }

        return i;
    }

    TRACE_1("There's no place for a new layer");
    return -1;
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
            layer.revoke();
        }
        else if (layer.state == Layer::State::Record)
        {
            layer.playback(_beat);
        }

        // do nothing if the layer is already in playback mode (let it keep playbacking)
        // this could happen when the button is kept being pressed even after the entire
        // recorded loop is over
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
            layer.revoke();
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

    TRACE_1("Going back to wandering as there are no recorded layers anymore");
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

        if (state == State::Record && previous == State::Wander)
        {
            TRACE_2("Starting to record at beat ", _beat);

            _recorded = _beat;

            for (auto & layer : _layers) // start recording all layers
            {
                if (layer.tag == -1)
                {
                    continue; // unused layer
                }

                layer.record(_beat);
            }
        }
        else if (state == State::Wander && previous != State::Wander)
        {
            TRACE_2("Starting to wander at beat ", _beat);

            _bars = 0; // reset the # of recorded bars

            for (auto & layer : _layers) // revoke all layers
            {
                if (layer.tag == -1)
                {
                    continue; // unused layer
                }

                layer.revoke();
            }

            callback(-1); // clear the bar
        }
#ifdef DEBUG
        else if (state == State::Playback && previous != State::Playback)
        {
            TRACE_4("Starting to playback ", (int)_bars, " recorded bars at beat ", _beat);
        }
        else if (state == State::Overlay && previous != State::Overlay)
        {
            TRACE_2("Starting to overlay at beat ", _beat);
        }
#endif

        if (state == State::Record || state == State::Playback || state == State::Overlay)
        {
            const auto difference = _beat - _recorded;

            if (difference.subdivisions == 0)
            {
                if (state == State::Record && _bars < Time::Bars) // still recording and haven't reached the max # of bars yet
                {
                    ++_bars; // increase the # of recorded bars when (recording and) entering a new bar

                    TRACE_3("Now recording bar #", (int)_bars, " for the first time");
                }

                if (difference.bars == _bars) // just passed the # of recorded bars
                {
                    TRACE_4("Resetting beat from ", _beat, " to ", _recorded);

                    _beat.bar = _recorded.bar;

                    // let all the layers know that the beat has changed

                    for (auto & layer : _layers)
                    {
                        if (layer.tag == -1)
                        {
                            continue; // unused layer
                        }

                        layer.click(_beat);
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

            if (!layer.played(_beat))
            {
                continue;
            }

            unsigned index;
            if (!rhythm::played(_config.rhythm, layer, _beat, /* out */ index))
            {
                continue;
            }

#ifndef DEBUG
            midi::play(
                _config.note + _config.accidental,
                _config.octave,
                _config.mode,
                layer.chord,
                style::degree(_config.style, index)
            );
#endif
        }

        ++_beat;

        for (auto & layer : _layers)
        {
            if (layer.tag == -1)
            {
                continue; // unused layer
            }

            layer.click(_beat);
        }

        previous = state;

        // enable interrupts as we are done with the main logic and no need for locks anymore
        interrupts();

        delay(60.f / static_cast<float>(_config.bpm) * 1000.f / static_cast<float>(Time::Subdivisions));
    }
}

} // midiate

#include "looper.h"

#include "../debug/debug.h"
#include "../midi/midi.h"
#include "../style/style.h"

#include <Arduino.h>

namespace midier
{

char Looper::start(Degree degree)
{
    for (char i = 0; i < sizeof(layers) / sizeof(Layer); ++i)
    {
        auto & layer = layers[i];

        if (layer.tag != -1)
        {
            continue; // this layer is used
        }

        auto start = Time::now;

        if (started.bar == -1)
        {
            TRACE_1("First layer starting");
            started = Time::now;
        }
        else if (assist != Assist::No)
        {
            const auto base = rhythm::base(config.rhythm);
            const auto jumps = base * (unsigned)assist;
            const auto round = Time::Subdivisions / jumps;

            while (((start - started).subdivisions % round) != 0)
            {
                ++start;
            }
        }

        layer = Layer(i, degree, start);

        if (state == State::Record || state == State::Overlay)
        {
            layer.record();
        }

        // we were in pre-record state and the first layer was just played
        // changing the state to `Record` will cause to actually start recording at
        // the next click of `run()` and will record this newly inserted layer as well
        if (state == State::Prerecord)
        {
            TRACE_1("Will record after prerecord");
            state = State::Record;
        }

        return i;
    }

    TRACE_1("There's no place for a new layer");
    return -1;
}

void Looper::stop(char tag)
{
    for (auto & layer : layers)
    {
        if (layer.tag != tag)
        {
            continue;
        }

        if (layer.state == Layer::State::Wait || layer.state == Layer::State::Wander)
        {
            layer.revoke();
        }
        else if (layer.state == Layer::State::Record)
        {
            layer.playback();
        }

        // do nothing if the layer is already in playback mode (let it keep playbacking)
        // this could happen when the button is kept being pressed even after the entire
        // recorded loop is over
    }
}

void Looper::revoke(char tag)
{
    if (state == State::Wander)
    {
        return; // nothing to do when wandering
    }

    if (tag == -1)
    {
        // we want to revoke the last recorded (or being recorded) layer
        // we cannot tell for sure which layer was the last one to be recorded,
        // so we assume it is the layer with the highest tag (and the highest index)

        for (unsigned i = sizeof(layers) / sizeof(Layer); i > 0; --i)
        {
            auto & layer = layers[i - 1];

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
    }
    else
    {
        for (auto & layer : layers)
        {
            if (layer.tag == tag)
            {
                layer.revoke();
                break;
            }
        }
    }

    // now check if there is no more recorded layers

    for (const auto & layer : layers)
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
        // both 'state' and the 'layers' may be modified, and 'beat' may be accessed via interrupts
        noInterrupts();

        if (started.bar != -1 && started.subdivision == Time::now.subdivision) // check if we should reset 'started'
        {
            bool reset = true;

            for (auto & layer : layers)
            {
                if (layer.tag != -1)
                {
                    reset = false; // don't reset if there exists any valid layer
                    break;
                }
            }

            if (reset)
            {
                TRACE_1("Reseting start beat as no more layers are being played");
                started.bar = -1;
            }
        }

        if (state == State::Record && previous != State::Record)
        {
            TRACE_1("Starting to record");

            recorded = Time::now;

            for (auto & layer : layers) // start recording all layers
            {
                if (layer.tag == -1)
                {
                    continue; // unused layer
                }

                layer.record();
            }
        }
        else if (state == State::Wander && previous != State::Wander)
        {
            TRACE_1("Starting to wander");

            bars = 0; // reset the # of recorded bars

            for (auto & layer : layers) // revoke all layers
            {
                if (layer.tag == -1)
                {
                    continue; // unused layer
                }

                layer.revoke();
            }

            callback(-1); // clear the bar
        }
        else if (state == State::Playback && previous != State::Playback)
        {
            TRACE_3("Starting to playback ", (int)bars, " recorded bars");
        }
        else if (state == State::Overlay && previous != State::Overlay)
        {
            TRACE_1("Starting to overlay");

            for (auto & layer : layers) // start recording all layers that are in wander mode
            {
                if (layer.tag == -1)
                {
                    continue; // unused layer
                }

                if (layer.state == Layer::State::Wander)
                {
                    layer.record();
                }
            }
        }

        if (state == State::Record || state == State::Playback || state == State::Overlay)
        {
            const auto difference = Time::now - recorded;

            if (difference.subdivisions == 0)
            {
                if (state == State::Record && bars < Time::Bars) // still recording and haven't reached the max # of bars yet
                {
                    ++bars; // increase the # of recorded bars when (recording and) entering a new bar

                    TRACE_3("Recording bar #", (int)bars, " for the first time");
                }

                if (difference.bars == bars) // just passed the # of recorded bars
                {
                    TRACE_2("Resetting beat to ", recorded);

                    Time::now = recorded;

                    // let all the layers know that the beat has changed

                    for (auto & layer : layers)
                    {
                        if (layer.tag == -1)
                        {
                            continue; // unused layer
                        }

                        layer.click();
                    }
                }

                callback((Time::now - recorded).bars);
            }
        }

        for (auto & layer : layers)
        {
            if (layer.tag == -1)
            {
                continue; // unused layer
            }

            if (!layer.played())
            {
                continue;
            }

            const auto & config = layer.configured == Layer::Configured::Static ? layer.config : this->config;

            unsigned index;
            if (!rhythm::played(config.rhythm, layer, /* out */ index))
            {
                continue;
            }

            char steps = config.style.steps;

            if (config.style.looped)
            {
                steps = (steps * 2) - 2;
            }

            index %= steps;

            if (index >= config.style.steps)
            {
                index = config.style.steps - (index - config.style.steps + 1) - 1; // the respective mirrored index
            }

#ifndef DEBUG
            midi::play(
                config.note + config.accidental,
                config.octave,
                config.mode,
                layer.chord,
                style::degree(config.style.steps, config.style.perm, index)
            );
#endif
        }

        ++Time::now;

        for (auto & layer : layers)
        {
            if (layer.tag == -1)
            {
                continue; // unused layer
            }

            layer.click();
        }

        previous = state;

        // enable interrupts as we are done with the main logic and no need for locks anymore
        interrupts();

        delay(60.f / static_cast<float>(bpm) * 1000.f / static_cast<float>(Time::Subdivisions));
    }
}

} // midier

#include "sequencer.h"

#include "../debug/debug.h"

#include <Arduino.h>

namespace midier
{

Sequencer::Sequencer(ILayers layers, unsigned char bpm) :
    layers(layers),
    bpm(bpm)
{}

Sequencer::Handle Sequencer::start(Degree degree)
{
    Sequencer::Handle handle;

    for (char i = 0; i < layers.count(); ++i)
    {
        auto & layer = layers[i];

        if (layer.tag != -1)
        {
            continue; // this layer is used
        }

        auto start = Time::now;

        if (_started.bar == -1)
        {
            TRACE_1(F("First layer starting"));
            _started = Time::now;
        }
        else if (assist != Assist::No)
        {
            const auto jumps = (unsigned)assist;
            const auto round = Time::Subdivisions / jumps;

            while (((start - _started).subdivisions % round) != 0)
            {
                ++start;
            }
        }

        layer = Layer(i, degree, start);

        if (layers.config != nullptr)
        {
            // all layers share common configuration by default
            layer.config = layers.config;
        }

        if (state == State::Record || state == State::Overlay)
        {
            layer.record();
        }

        handle._layer = &layer;
        break;
    }

    if (handle._layer == nullptr)
    {
        TRACE_1(F("There's no place for a new layer"));
    }

    return handle;
}

void Sequencer::stop(Handle handle)
{
    if (handle._layer != nullptr)
    {
        handle._layer->stop();
    }
    else
    {
        TRACE_1(F("Requested to stop `nullptr` handle"));
    }
}

void Sequencer::revoke()
{
    // we want to revoke the last recorded (or being recorded) layer
    // we cannot tell for sure which layer was the last one to be recorded,
    // so we assume it is the layer with the highest tag (and the highest index)

    for (unsigned i = layers.count(); i > 0; --i)
    {
        auto & layer = layers[i - 1];

        if (layer.tag == -1)
        {
            continue;
        }

        if (layer.state == Layer::State::Record || layer.state == Layer::State::Playback)
        {
            TRACE_3(F("Layer "), layer, F(" is probably the last recorded one"));
            layer.revoke();
            break;
        }
    }
}

void Sequencer::record()
{
    if (state == State::Wander)
    {
        state = State::Prerecord;
    }
    else if (state == State::Prerecord)
    {
        state = State::Wander;
    }
    else if (state == State::Record || state == State::Overlay)
    {
        state = State::Playback;
    }
    else if (state == State::Playback)
    {
        state = State::Overlay;
    }
}

void Sequencer::wander()
{
    state = State::Wander;
}

Sequencer::Bar Sequencer::click(Run run)
{
    const auto bps = (float)bpm / 60.f; // beats per second
    const auto mspb = 1000.f / bps; // ms per beat
    const auto mspc = mspb / (float)midier::Time::Subdivisions; // ms per click

    if (_clicked == -1)
    {
        // this is the very first click so no need to wait
    }
    else
    {
        if (run == Run::Sync)
        {
            while (millis() - _clicked < mspc); // wait until enough time has passed
        }
        else if (run == Run::Async)
        {
            if (millis() - _clicked < mspc)
            {
                return Bar::Same; // we don't actually click yet
            }
        }
    }

    _clicked = millis(); // reset the time of the last click to now

    // only now we are actually starting to click

    Bar bar = Bar::Same;

    if (_started.bar != -1) // should we reset 'started'?
    {
        if (_started.subdivision == Time::now.subdivision && layers.idle())
        {
            TRACE_1(F("Reseting start beat as no more layers are being played"));
            _started.bar = -1;
        }
    }

    if (state == State::Record || state == State::Playback || state == State::Overlay) // check if should go back to wandering
    {
        if (layers.all([](const Layer & layer){ return layer.state == Layer::State::Wander; }))
        {
            TRACE_1(F("Going back to wandering as there are no recorded layers anymore"));
            state = State::Wander;
        }
    }

    if (state == State::Prerecord) // check if should start recording
    {
        if (layers.used() > 0) // some layer has started since we were marked for prerecording
        {
            TRACE_1(F("Will record after prerecord"));
            state = State::Record;
        }
    }

    if (state != _previous) // the state has changed since the last click
    {
        if (state == State::Prerecord)
        {
            TRACE_1(F("Marked for pre-record"));
        }
        else if (state == State::Record)
        {
            TRACE_1(F("Starting to record"));

            _record.when = Time::now;
            _record.bars = 0; // reset the number of bars recorded

            layers.record();
        }
        else if (state == State::Wander)
        {
            TRACE_1(F("Starting to wander"));

            layers.revoke();

            if (_previous != State::Prerecord)
            {
                bar = Bar::None;
            }
        }
        else if (state == State::Playback)
        {
            TRACE_3(F("Starting to playback "), (int)_record.bars, F(" recorded bars"));
        }
        else if (state == State::Overlay)
        {
            TRACE_1(F("Starting to overlay"));

            layers.eval([](Layer & layer)
                {
                    if (layer.state == Layer::State::Wander)
                    {
                        layer.record();
                    }
                });
        }

        _previous = state; // save the state after we finished comparing it
    }

    if (state == State::Record || state == State::Playback || state == State::Overlay)
    {
        const auto difference = Time::now - _record.when;

        if (difference.subdivisions == 0)
        {
            if (state == State::Record)
            {
                if (_record.bars < Time::Bars)
                {
                    ++_record.bars; // we count recorded bars from the time they start
                    TRACE_3(F("Recording bar #"), (int)_record.bars, F(" for the first time"));
                }
                else
                {
                    TRACE_3(F("Recorded maximum of "), (int)_record.bars, F(" bars"));
                    _previous = state = State::Overlay; // setting `_previous` also for it to not count as a state change
                }
            }
            else // playback or overlay
            {
                if (difference.bars == _record.bars) // just passed the # of recorded bars
                {
                    TRACE_5(F("Resetting beat to "), _record.when, F(" after "), (int)difference.bars, F(" bars"));
                    Time::now = _record.when;
                }
            }

            // using `Time::now` and not `difference` to support the cases when we reset the beat
            bar = (Bar)((Time::now - _record.when).bars + 1);
        }
    }

    // let all layers click
    layers.click();

    // after playing all the layers, we advance the global time
    ++Time::now;

    // let the client know if the bar has changed
    return bar;
}

void Sequencer::run(const Time::Duration & duration)
{
    unsigned subdivisions = duration.total();

    while (subdivisions-- > 0)
    {
        click(Run::Sync);
    }
}

} // midier

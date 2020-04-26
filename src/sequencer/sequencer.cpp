#include "sequencer.h"

#include "../debug/debug.h"

#include <Arduino.h>

namespace midier
{

Sequencer::Sequencer(ILayers layers) : Sequencer(layers, Config { /* default configuration */ })
{}

Sequencer::Sequencer(ILayers layers, const Config & config) : Sequencer(layers, config, 60)
{}

Sequencer::Sequencer(ILayers layers, unsigned char bpm) : Sequencer(layers, Config { /* default configuration */ }, bpm)
{}

Sequencer::Sequencer(ILayers layers, const Config & config, unsigned char bpm) :
    layers(layers),
    bpm(bpm),
    config(config)
{}

bool Sequencer::recording() const
{
    return _state == State::Prerecord || _state == State::Record || _state == State::Overlay;
}

Sequencer::Handle Sequencer::start(Degree degree)
{
    Sequencer::Handle handle;

    for (char i = 0; i < layers.count(); ++i)
    {
        auto & layer = layers[i];

        if (layer.running())
        {
            continue;
        }

        unsigned char delay = 0;

        if (_started == -1)
        {
            TRACE_1(F("First layer starting now"));
            _started = Time::now.subdivision;
        }
        else if (assist != Assist::No)
        {
            // what's the rate of the common rhythm
            const auto rate = rhythm::rate(config.rhythm());

            // how many (assisted) units are there in a single bar
            const auto units = (unsigned)rate * (unsigned)assist;

            // how many subdivisions every unit takes
            const auto jumps = Time::Subdivisions / units;

            // how many subdivisions passed since the last jump
            const auto passed = (Time::now - Time { .bars = 0, .subdivisions = _started }).subdivisions % jumps;

            // how many subdivisions are left until the next jump
            delay = (jumps - passed) % jumps;
        }

        layer = Layer(
#ifdef DEBUG
            i, // layer id
#endif
            degree, delay);

        // all layers share common configuration by default
        layer.config = &config;

        if (_state == State::Record || _state == State::Overlay)
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
    else
    {
        handle.config = &handle._layer->config;
    }

    return handle;
}

Sequencer::Handle Sequencer::start(Degree degree, const Config & config)
{
    const Handle handle = start(degree);

    if (handle._layer != nullptr)
    {
        handle._layer->config = config;
    }

    return handle;
}

void Sequencer::stop(Handle handle)
{
    if (handle._layer != nullptr)
    {
        if (handle._layer->running())
        {
            handle._layer->stop();
        }
        else
        {
            // this could happen if the layer was manually revoked while still
            // being pressed and now it's stopped being pressed
            TRACE_1(F("Requested to stop an idle layer"));
        }
    }
    else
    {
        // this could happen if there was no place for more layers
        TRACE_1(F("Requested to stop `nullptr` handle"));
    }
}

void Sequencer::revoke()
{
    // we want to revoke the last recorded (or being recorded) layer
    // we cannot tell for sure which layer was the last one to be recorded,
    // so we assume it is the layer in the highest index

    for (unsigned i = layers.count(); i > 0; --i)
    {
        auto & layer = layers[i - 1];

        if (layer.idle())
        {
            continue;
        }

        if (layer.looping())
        {
            TRACE_3(F("Layer "), layer, F(" is probably the last recorded one"));
            layer.revoke();
            break;
        }
    }
}

void Sequencer::record()
{
    if (_state == State::Wander)
    {
        _state = State::Prerecord;
    }
    else if (_state == State::Prerecord)
    {
        _state = State::Wander;
    }
    else if (_state == State::Record || _state == State::Overlay)
    {
        _state = State::Playback;
    }
    else if (_state == State::Playback)
    {
        _state = State::Overlay;
    }
}

void Sequencer::wander()
{
    _state = State::Wander;
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

    if (_started != -1) // check if we should reset `_started`
    {
        if (_started == Time::now.subdivision && layers.idle())
        {
            TRACE_1(F("Reseting start beat as no more layers are being played"));
            _started = -1;
        }
    }

    if (_state == State::Record || _state == State::Playback || _state == State::Overlay) // check if should go back to wandering
    {
        if (layers.all([](const Layer & layer){ return layer.wandering(); }))
        {
            TRACE_1(F("Going back to wandering as there are no recorded layers anymore"));
            _state = State::Wander;
        }
    }

    if (_state == State::Prerecord) // check if should start recording
    {
        if (layers.running() > 0) // some layer has started since we were marked for prerecording
        {
            _state = State::Record;
        }
    }

    if (_state != _previous) // the state has changed since the last click
    {
        if (_state == State::Prerecord)
        {
            TRACE_1(F("Marked for pre-record"));
        }
        else if (_state == State::Record)
        {
            TRACE_1(F("Starting to record"));

            _record.when = Time::now;
            _record.bars = 0; // reset the number of bars recorded

            layers.record();
        }
        else if (_state == State::Wander)
        {
            TRACE_1(F("Starting to wander"));

            layers.revoke();

            if (_previous != State::Prerecord)
            {
                bar = Bar::None;
            }
        }
        else if (_state == State::Playback)
        {
            TRACE_3(F("Starting to playback "), (int)_record.bars, F(" recorded bars"));
        }
        else if (_state == State::Overlay)
        {
            TRACE_1(F("Starting to overlay"));

            layers.eval([](Layer & layer)
                {
                    if (layer.waiting() || layer.wandering())
                    {
                        layer.record();
                    }
                });
        }

        _previous = _state; // save the state after we finished comparing it
    }

    if (_state == State::Record || _state == State::Playback || _state == State::Overlay)
    {
        const auto difference = Time::now - _record.when;

        if (difference.subdivisions == 0)
        {
            if (_state == State::Record)
            {
                if (_record.bars < Time::Bars)
                {
                    ++_record.bars; // we count recorded bars from the time they start
                    TRACE_3(F("Recording bar #"), (int)_record.bars, F(" for the first time"));
                }
                else
                {
                    TRACE_3(F("Recorded maximum of "), (int)_record.bars, F(" bars"));
                    _previous = _state = State::Overlay; // setting `_previous` also for it to not count as a state change
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

void Sequencer::run(float bars)
{
    const unsigned integer = (unsigned)bars;
    const float fractional = (bars - (long)bars);

    run({ .bars = integer, .subdivisions = fractional * Time::Subdivisions });
}

void Sequencer::play(Degree degree, const Time::Duration & duration)
{
    const Handle handle = start(degree);
    run(duration);
    stop(handle);
}

void Sequencer::play(Degree degree, const Time::Duration & duration, const Config & config)
{
    const Handle handle = start(degree, config);
    run(duration);
    stop(handle);
}

} // midier

#include "layer.h"

#include "../scale/scale.h"
#include "../style/style.h"
#include "../triad/triad.h"

namespace midier
{

Layer::Layer(
#ifdef DEBUG
    unsigned char id,
#endif
    Degree chord,
    unsigned char delay,
    Config * config) :
#ifdef DEBUG
    id(id),
#endif
    chord(chord),
    start({ .bar = -1, .subdivision = delay }), // we use `start` to hold `delay` as it will not be used until the layer will actually start
    config(config), // all layers share common configuration by default
    _state(State::Wait)
{
    TRACE_7(F("New layer "), *this, F(" of scale degree "), chord, F(" will start in "), (int)start.subdivision, F(" subdivisions"));
}

bool Layer::idle() const
{
    return _state == State::Idle;
}

bool Layer::running() const
{
    return _state != State::Idle;
}

bool Layer::waiting() const
{
    return _state == State::Wait;
}

bool Layer::wandering() const
{
    return _state == State::Wander;
}

bool Layer::looping() const
{
    return _state == State::Record || _state == State::Playback;
}

void Layer::stop()
{
    if (_state == Layer::State::Wait || _state == Layer::State::Wander)
    {
        revoke();
    }
    else if (_state == Layer::State::Record)
    {
        TRACE_3(F("Marking layer "), *this, F(" for playback"));
        _state = State::Playback;
    }

    // do nothing if the layer is already in playback mode (let it keep playbacking)
    // this could happen when the button is kept being pressed even after the entire
    // recorded loop is over
}

void Layer::record()
{
    if (_state == State::Wait)
    {
        TRACE_3(F("Marking non-started layer "), *this, F(" for record"));
        _loop.bar = 0; // this should indicate `click()` to start recording when layer will be started
    }
    else
    {
        TRACE_3(F("Marking layer "), *this, F(" for record"));
        _state = State::Record;
    }
}

void Layer::revoke()
{
    TRACE_2(F("Revoking layer "), *this);

    if (_played.subdivisions != -1)
    {
        TRACE_2(F("A note is still being played for revoked layer "), *this);
        midi::off(_played.number); // stop the note that is still being played by this layer
    }

    _state = State::Idle; // will affect immediately
}

void Layer::click()
{
    if (_played.subdivisions != -1) // check if a note is being played and should stop
    {
        // what's the rate of the rhythm?
        const auto rate = rhythm::rate(config->rhythm());

        // how many units of such rate exist in a bar (1/4)?
        const auto count = (unsigned)rate;

        // how many subdivisions exist between every two units?
        const auto subdivisions = (unsigned)(Time::Subdivisions / count);

        if (++_played.subdivisions >= subdivisions)
        {
            midi::off(_played.number);
            _played.subdivisions = -1;
        }
    }

    if (_state == State::Playback)
    {
        if (_loop.end.bar == -1) // check if was just marked for playback
        {
            TRACE_2(F("Stopping to record layer "), *this);
            _loop.end = Time::now;
        }

        if (Time::now == _loop.end) // check if should exit the playback loop now
        {
            if (_loop.end != _loop.start)
            {
                TRACE_3(F("Layer "), *this, F(" is exiting the playback loop"));
                bar = -1; // we have reached the end of the recording of this finite layer
            }
            else
            {
                // the layer is infinite and therefore kept continuous
            }
        }
    }

    if (_state == State::Wait && start.subdivision-- == 0) // check if waiting and should start now
    {
        TRACE_2(F("Starting layer "), *this);

        start = Time::now;
        _state = State::Wander;

        if (_loop.bar == 0) // the layer was marked for recording upon starting
        {
            TRACE_2(F("Immediately recording layer "), *this);
            _state = State::Record;
        }
    }

    if (_state == State::Wander || _state == State::Record || _state == State::Playback) // check if should advance `bar`
    {
        if (_state == State::Playback && bar == -1)
        {
            // no need to advance `bar` as we are in playback mode and out of the loop
        }
        else if (Time::now.subdivision == start.subdivision)
        {
            bar = (bar + 1) % Layer::Period; // keep everything going as we are still continuous
            TRACE_4(F("Layer "), *this, F(" is entering bar #"), bar);
        }
    }

    if (_state == State::Record)
    {
        if (_loop.start.bar == -1) // check if was just marked for record
        {
            _loop.start = Time::now;
            _loop.bar = bar;

            TRACE_4(F("Starting to record layer "), *this, F(" at bar "), bar);
        }
        else if (Time::now == _loop.start) // check if this layer is infinite
        {
            // the layer was recorded throughout the entire record loop

            _state = State::Playback;
            _loop.end = Time::now;

            TRACE_3(F("Layer "), *this, F(" is infinite"));
        }
    }

    if (_state == State::Playback && Time::now == _loop.start) // check if we are out of the playback loop and should enter now
    {
        if (bar == -1)
        {
            TRACE_4(F("Layer "), *this, F(" is entering the playback loop with bar "), _loop.bar);
            bar = _loop.bar;
        }
        else
        {
            // the layer is infinite
        }
    }

    // now, after checking all the stuff above, play a note if you want to

    if (_state == State::Wait)
    {
        return; // not started yet
    }

    if (_state == State::Playback && bar == -1)
    {
        return; // in playback mode and out of loop
    }

    unsigned index;
    if (!rhythm::played(config->rhythm(), *this, /* out */ index))
    {
        return; // not playing right now rhythmically
    }

    char steps = config->steps();

    if (config->looped())
    {
        steps = (steps * 2) - 2;
    }

    index %= steps;

    if (index >= config->steps())
    {
        index = config->steps() - (index - config->steps() + 1) - 1; // the respective mirrored index
    }

    const auto note = config->note() + config->accidental()
        + scale::interval(config->mode(), chord)
        + triad::interval(
            scale::quality(config->mode(), chord),
            style::degree(config->steps(), config->perm(), index));

    if (_played.subdivisions != -1)
    {
        TRACE_2(F("A note is still being played for layer "), *this);

        // at most one note can be played at a time by every layer
        // if there still is a note being played because of this layer, we
        // stop playing it right before starting to play the new note
        midi::off(_played.number);
    }

    _played.subdivisions = 0; // start counting the number of clicks the note is playing
    _played.number = midi::number(note, config->octave());

    midi::on(_played.number, (unsigned)volume);
}

} // midier

#include "layer.h"

#include "../debug/debug.h"
#include "../scale/scale.h"
#include "../style/style.h"
#include "../triad/triad.h"

namespace midier
{

Layer::Config::Config() :
    _data(),
    _view(&_data)
{}

Layer::Config::Config(Config && config)
{
    *this = (Config &&)config;
}

Layer::Config & Layer::Config::operator=(Config && other)
{
    _data = other._data;

    if (other._view == &other._data)
    {
        _view = &_data;
    }
    else
    {
        _view = other._view;
    }

    return *this;
}

Layer::Layer(Tag tag, Degree chord, const Time & start) :
    tag(tag),
    chord(chord),
    start(start)
{
    if (start == Time::now)
    {
        TRACE_4(F("Starting layer "), *this, F(" of scale degree "), chord);
        state = State::Wander;
    }
    else
    {
        TRACE_6(F("Adding layer "), *this, F(" of scale degree "), chord, F(" to start at future beat "), start);
        state = State::Wait;
    }
}

void Layer::record()
{
    if (state == State::Wait)
    {
        TRACE_3(F("Marking non-started layer "), *this, F(" for record"));
        loop.bar = 0; // this should indicate `click()` to start recording when layer will be started
    }
    else
    {
        TRACE_3(F("Marking layer "), *this, F(" for record"));
        state = State::Record;
        loop.bar = -1;
    }
}

void Layer::playback()
{
    if (Time::now == loop.start)
    {
        // if the layer was to complete a full loop, it would have been set to playback
        // state by `click()` and this very method would not have been called

        TRACE_3(F("Layer "), *this, F(" was stopped before started to record"));
        revoke();
    }
    else
    {
        TRACE_2(F("Stopping to record finite layer "), *this);

        state = State::Playback;
        loop.end = Time::now; // the layer will not be played from the very next click
    }
}

void Layer::revoke()
{
    TRACE_2(F("Revoking layer "), *this);

    if (played.subdivisions != -1)
    {
        TRACE_2(F("A note is still being played for revoked layer "), *this);
        midi::off(played.number); // stop the note that is still being played by this layer
    }

    tag = -1;
}

void Layer::click()
{
    if (state == State::Wait && Time::now == start) // check if waiting and should start now
    {
        TRACE_2(F("Starting layer "), *this);
        state = State::Wander;

        if (loop.bar == 0) // the layer was marked for recording upon starting
        {
            TRACE_2(F("Immediately recording layer "), *this);
            record();
        }
    }

    if (played.subdivisions != -1) // check if a note is being played and should stop
    {
        // what's the rate of the rhythm?
        const auto rate = rhythm::rate(config->rhythm());

        // how many units of such rate exist in a bar (1/4)?
        const auto count = (unsigned)rate;

        // how many subdivisions exist between every two units?
        const auto subdivisions = (unsigned)(Time::Subdivisions / count);

        if (++played.subdivisions >= subdivisions)
        {
            midi::off(played.number);
            played.subdivisions = -1;
        }
    }

    if (state == State::Wander || state == State::Record || state == State::Playback) // check if should advance `bar`
    {
        if (state == State::Playback && bar == -1)
        {
            // no need to advance `bar` as we are in playback mode and out of the loop
        }
        else if (Time::now.subdivision == start.subdivision)
        {
            bar = (bar + 1) % Layer::Period; // keep everything going as we are still continuous
            TRACE_4(F("Layer "), *this, F(" is entering bar #"), bar);
        }
    }

    if (state == State::Record)
    {
        if (loop.bar == -1) // check if the layer was just marked for record
        {
            // only now we record `loop.start` and `loop.bar` since both
            // `Time::now` and `bar` could have changed since the marking
            loop.start = Time::now;
            loop.bar = bar;

            TRACE_4(F("Starting to record layer "), *this, F(" at bar "), bar);
        }
        else if (Time::now == loop.start) // check if this layer is infinite
        {
            // the layer was recorded throughout the entire record loop

            state = State::Playback;
            loop.end = Time::now;

            TRACE_3(F("Layer "), *this, F(" is infinite"));
        }
    }

    if (state == State::Playback && Time::now == loop.end) // check if should exit the playback loop now
    {
        if (loop.end != loop.start)
        {
            TRACE_3(F("Layer "), *this, F(" is exiting the playback loop"));
            bar = -1; // we have reached the end of the recording of this finite layer
        }
        else
        {
            // the layer is infinite and therefore kept continuous
        }
    }

    if (state == State::Playback && Time::now == loop.start) // check if we are out of the playback loop and should enter now
    {
        if (bar == -1)
        {
            TRACE_4(F("Layer "), *this, F(" is entering the playback loop with bar "), loop.bar);
            bar = loop.bar;
        }
        else
        {
            // the layer is infinite
        }
    }

    // now, after checking all the stuff above, play a note if you want to

    if (state == State::Wait)
    {
        return; // not started yet
    }

    if (state == State::Playback && bar == -1)
    {
        return; // in playback mode and out of loop
    }

    unsigned index;
    if (!rhythm::played(config->rhythm(), *this, /* out */ index))
    {
        return; // not playing right now rhythmically
    }

    char steps = config->style.steps();

    if (config->style.looped())
    {
        steps = (steps * 2) - 2;
    }

    index %= steps;

    if (index >= config->style.steps())
    {
        index = config->style.steps() - (index - config->style.steps() + 1) - 1; // the respective mirrored index
    }

    const auto note = config->note() + config->accidental()
        + scale::interval(config->mode(), chord)
        + triad::interval(
            scale::quality(config->mode(), chord),
            style::degree(config->style.steps(), config->style.perm(), index));

    if (played.subdivisions != -1)
    {
        TRACE_2(F("A note is still being played for layer "), *this);

        // at most one note can be played at a time by every layer
        // if there still is a note being played because of this layer, we
        // stop playing it right before starting to play the new note
        midi::off(played.number);
    }

    played.subdivisions = 0; // start counting the number of clicks the note is playing
    played.number = midi::number(note, config->octave());

    midi::on(played.number);
}

} // midier

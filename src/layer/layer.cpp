#include "layer.h"

#include "../debug/debug.h"

namespace midier
{

Layer::Layer(char tag, Degree chord, const Time & start) :
    tag(tag),
    chord(chord),
    start(start)
{
    if (start == Time::now)
    {
        TRACE_4("Starting layer ", *this, " of scale degree ", chord);
        state = State::Wander;
    }
    else
    {
        TRACE_6("Adding layer ", *this, " of scale degree ", chord, " to start at future beat ", start);
        state = State::Wait;
    }
}

void Layer::record()
{
    if (state == State::Wait)
    {
        TRACE_3("Non-started layer ", *this, " is marked for recording");
        loop.bar = 0; // this should indicate `click()` to start recording when layer will be started
    }
    else
    {
        TRACE_4("Starting to record layer ", *this, " at bar #", bar);
        state = State::Record;
        loop.start = Time::now;
        loop.bar = bar;
    }
}

void Layer::playback()
{
    if (Time::now == loop.start)
    {
        // if the layer was to complete a full loop, it would have been set to playback
        // state by `click()` and this very method would not have been called

        TRACE_3("Layer ", *this, " was stopped before started to record");
        revoke();
    }
    else
    {
        TRACE_2("Stopping to record finite layer ", *this);

        state = State::Playback;
        loop.end = Time::now;
        bar = -1; // exit the record loop of this finite layer
    }
}

void Layer::click()
{
    if (state == State::Wait)
    {
        if (Time::now == start)
        {
            TRACE_2("Starting layer ", *this);
            state = State::Wander;

            if (loop.bar == 0) // the layer was marked for recording upon starting
            {
                TRACE_2("Immediately recording layer ", *this);
                record();
            }
        }
    }
    else
    {
        if (state == State::Record && Time::now == loop.start)
        {
            // the layer was recorded throughout the entire record loop

            state = State::Playback;
            loop.end = Time::now;

            TRACE_3("Layer ", *this, " is infinite");
        }

        if (state == State::Playback && Time::now == loop.end)
        {
            if (loop.end != loop.start)
            {
                bar = -1; // we have reached the end of the recording of this finite layer
            }
            else
            {
                // we keep infinite layers continuous
            }
        }

        if (bar == -1) // we are in playback mode and out of the loop
        {
            if (Time::now == loop.start) // re-entering the recorded loop
            {
                bar = loop.bar;

                TRACE_4("Layer ", *this, " entering recorded loop with bar ", loop.bar);
            }
#ifdef DEBUG
            else if ((Time::now - start).subdivisions == 0)
            {
                TRACE_3("Layer ", *this, " is out of loop");
            }
#endif
        }
        else if ((Time::now - start).subdivisions == 0) // using 'else' to make sure we don't change the bar exactly after being restored
        {
            bar = (bar + 1) % Layer::Period; // keep everything going as we are still continuous
            TRACE_4("Layer ", *this, " is entering bar #", bar);
        }
    }
}

void Layer::revoke()
{
    TRACE_2("Revoking layer ", *this);
    tag = -1;
}

bool Layer::played()
{
    if (state == State::Wait)
    {
        return false; // not started yet
    }

    if (bar == -1) // we are in playback mode and out of the loop
    {
        return false;
    }

    return true;
}

} // midier

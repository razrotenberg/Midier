#include "layer.h"

#include "debug.h"

namespace midiate
{

Layer::Layer(char tag, Degree chord, const Time & now, const Time & start) :
    tag(tag),
    chord(chord),
    start(start)
{
    if (start == now)
    {
        TRACE_6("Starting layer ", *this, " of scale degree ", chord, " now at beat ", now);
        state = State::Wander;
    }
    else
    {
        TRACE_8("Adding layer ", *this, " of scale degree ", chord, " to be started at future beat ", start, " and now it's ", now);
        state = State::Wait;
    }
}

void Layer::record(const Time & now)
{
    if (state == State::Wait)
    {
        TRACE_4("Non-started layer ", *this, " is marked for recording on start at beat ", now);
        loop.bar = 0; // this should indicate `click()` to start recording when layer will be started
    }
    else
    {
        TRACE_6("Starting to record layer ", *this, " (bar #", bar, ") at beat ", now);
        state = State::Record;
        loop.start = now;
        loop.bar = bar;
    }
}

void Layer::playback(const Time & now)
{
    if (now == loop.start)
    {
        // if the layer was to complete a full loop, it would have been set to playback
        // state by `click()` and this very method would not have been called

        TRACE_3("Layer ", *this, " was stopped before started to record");
        revoke();
    }
    else
    {
        TRACE_4("Stopping to record finite layer ", *this, " at beat ", now);

        state = State::Playback;
        loop.end = now;
        bar = -1; // exit the record loop of this finite layer
    }
}

void Layer::click(const Time & now)
{
    if (state == State::Wait)
    {
        if (now == start)
        {
            TRACE_4("Starting layer ", *this, " now at beat ", now);
            state = State::Wander;

            if (loop.bar == 0) // the layer was marked for recording upon starting
            {
                TRACE_2("Immediately recording layer ", *this);
                record(now);
            }
        }
    }
    else
    {
        if (state == State::Record && now == loop.start)
        {
            // the layer was recorded throughout the entire record loop

            state = State::Playback;
            loop.end = now;

            TRACE_3("Layer ", *this, " is infinite");
        }

        if (state == State::Playback && now == loop.end)
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
            if (now == loop.start) // re-entering the recorded loop
            {
                bar = loop.bar;

                TRACE_6("Resetting layer ", *this, " bar to ", loop.bar, " as it is entering the recorded loop at ", now);
            }
#ifdef DEBUG
            else if ((now - start).subdivisions == 0)
            {
                TRACE_4("Layer ", *this, " is out of loop at beat ", now);
            }
#endif
        }
        else if ((now - start).subdivisions == 0) // using 'else' to make sure we don't change the bar exactly after being restored
        {
            bar = (bar + 1) % Layer::Period; // keep everything going as we are still continuous
            TRACE_6("Layer ", *this, " is entering bar #", bar, " at beat ", now);
        }
    }
}

void Layer::revoke()
{
    TRACE_2("Revoking layer ", *this);
    tag = -1;
}

bool Layer::played(const Time & now)
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

} // midiate

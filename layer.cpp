#include "layer.h"

#include "debug.h"

namespace midiate
{

Layer::Layer(char tag, Degree chord, const Time & now) :
    tag(tag),
    chord(chord),
    start(now)
{}

void Layer::record(const Time & now)
{
    TRACE_6("Starting to record layer ", *this, " (bar #", bar, ") at beat ", now);

    state = State::Record;
    loop.start = now;
    loop.bar = bar;
}

void Layer::playback(const Time & now)
{
    // manually stopped recording layers are always finite (even if by some chance
    // they are being stopped right on 'loop.start')

    TRACE_4("Stopping to record finite layer ", *this, " at beat ", now);

    state = State::Playback;
    loop.end = now;
    bar = -1; // exit the record loop of this finite layer
}

void Layer::click(const Time & now)
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

void Layer::revoke()
{
    TRACE_2("Revoking layer ", *this);
    tag = -1;
}

bool Layer::played(const Time & now)
{
    if (state == State::Playback && Time::ordered(loop.start, now, loop.end) == false)
    {
        return false; // not supposed to be played right now
    }

    return true;
}

} // midiate

#pragma once

#include "../layers/layers.h"

namespace midier
{

struct Sequencer
{
    enum class State : char
    {
        Wander,
        Prerecord,
        Record,
        Playback,
        Overlay,
    };

    enum class Assist : char
    {
        No,

        // numbers represeting the # of subdivisions to round
        _12 = 12,
        _8 = 8,
        _6 = 6,
        _4 = 4,
        _3 = 3,
    };

    enum class Bar : char
    {
        None = -1,
        Same = 0,

        // bar index
    };

    enum class Run : char
    {
        Sync,
        Async,
    };

    // creation
    Sequencer(ILayers layers, unsigned char bpm = 60);

    // start and stop layers
    Layer::Tag start(Degree degree); // return corresponding tag or (-1) if could not play
    void stop(Layer::Tag tag);

    // revoke a recorded layer
    // passing (-1) as `tag` means to revoke the last recorded layer
    // doing nothing if wandering
    void revoke(Layer::Tag tag = -1);

    // state changes
    void record(); // toggle between record/playback/overlay modes
    void wander(); // go back to wandering

    // click the next subdivision in a bar and run the logic:
    //   1) manage state changes
    //   2) reset the beat if reached the end of the recorded loop
    //   3) click all layers
    //
    // this can be done either synchronously or asynchronously
    //
    // synchronous calls are blocking and wait for enough time to pass before
    // actually clicking the next subdivision in order for the bar to take
    // the correct amount of time according to `bpm`
    //
    // asynchronous calls are non-blocking and return `Bar::Same` if it's too
    // soon to actually click
    //
    // returns an indicator of any changes in the record loop bar or its index
    // if we are currently inside a record loop
    //
    Bar click(Run run);

    // run synchronously for a certain time duration
    // this method is blocking and returns after the time duration has fully passed
    void run(const Time::Duration & duration);

    // exposed members
    State state = State::Wander;
    Assist assist = Assist::No;
    ILayers layers;
    unsigned char bpm;

private:
    unsigned long long _clicked = -1; // timestamp of previous click

    struct {
        Time when; // when we started to record
        char bars; // # of recorded bars
    } _record;

    Time _started; // first note ever played
    State _previous = state;
};

} // midier

#pragma once

#include "../layers/layers.h"

namespace midier
{

struct Sequencer
{
    enum class Assist : char
    {
        No,

        Half = 2,
        Full = 1,
    };

    enum Bar : short
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

    // this class is made to hide the underlying `Layer` from the client
    // so he or she will not call `Layer` methods directly but will call
    // `Sequencer` methods only
    struct Handle
    {
        config::Viewed * config = nullptr;

    private:
        friend class Sequencer;
        Layer * _layer = nullptr;
    };

    // creation
    Sequencer(ILayers layers);
    Sequencer(ILayers layers, const Config & config);
    Sequencer(ILayers layers,                        unsigned char bpm);
    Sequencer(ILayers layers, const Config & config, unsigned char bpm);

    // queries
    bool recording() const;

    // start and stop layers
    Handle start(Degree degree);
    Handle start(Degree degree, const Config & config);
    void stop(Handle handle);

    // revoke the last recorded layer
    // doing nothing if wandering
    void revoke();

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
    // these methods are blocking and return after the time duration has fully passed
    void run(const Time::Duration & duration);

    // synchronously play a scale degree for a certain time duration
    // the scale degree is stopped at the end of the duration
    void play(Degree degree, const Time::Duration & duration);
    void play(Degree degree, const Time::Duration & duration, const Config & config);

    // exposed members
    Assist assist = Assist::No;
    ILayers layers;
    unsigned char bpm;
    Config config; // common layer configuration

private:
    enum class State : char
    {
        Wander,
        Prerecord,
        Record,
        Playback,
        Overlay,
    };

    unsigned long long _clicked = -1; // timestamp of previous click

    struct {
        Time when; // when we started to record
        char bars; // # of recorded bars
    } _record;

    // this is the subdivision of the first note ever played.
    // it is used to delay the start of new layers to match the rhythm when in assist mode.
    // we record full bars only. therefore, even if the time will be changed because of
    // resetting it at the end of the recorded loop, only the bar will change and the
    // subdivision will be kept the same. calculating delays in terms of subdivisions
    // is robust to beat changes and frugal in memory.
    char _started = -1;

    State _state = State::Wander;
    State _previous = _state;
};

} // midier

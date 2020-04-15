#include "player.h"

#include <Arduino.h>

namespace midier
{

namespace
{

struct Info
{
    enum class State : char
    {
        Waiting,
        Running,
        Finished,
    };

    State state = State::Waiting;
    Layer::Tag tag;
    Time started;
};

} //

Player::Player(Looper & looper, unsigned bpm) :
    _looper(looper),
    _bpm(bpm)
{}

void Player::play(const Recipe recipes[], unsigned count)
{
    Info infos[count];

    const auto start = Time::now;

    while (true)
    {
        // start and stop layers
        for (unsigned i = 0; i < count; ++i)
        {
            const Recipe & recipe = recipes[i];
            Info & info = infos[i];

            if (info.state == Info::State::Waiting)
            {
                if ((Time::now - start).total() == recipe.after * Time::Subdivisions)
                {
                    info.tag = _looper.start(recipe.degree);
                    info.started = Time::now;
                    info.state = Info::State::Running;

                    if (recipe.config != nullptr)
                    {
                        _looper.layers.find(info.tag)->config = recipe.config;
                    }
                }
            }
            else if (info.state == Info::State::Running)
            {
                if ((Time::now - info.started).total() == recipe.duration * Time::Subdivisions)
                {
                    _looper.stop(info.tag);
                    info.state = Info::State::Finished;
                }
            }
        }

        // check if all layers are finished
        bool finished = true;

        for (const auto & info : infos)
        {
            finished &= info.state == Info::State::Finished;
        }

        if (finished)
        {
            break; // we are done
        }

        // click the looper
        _looper.click();

        // sleep between subdivisions
        const auto bps = (float)_bpm / 60.f;
        const auto mspb = 1000.f / bps;
        const auto msps = mspb / (float)midier::Time::Subdivisions;

        delay(msps);
    }
}

} // midier

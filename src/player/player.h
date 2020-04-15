#pragma once

#include "../looper/looper.h"

namespace midier
{

struct Player
{
    struct Recipe
    {
        Degree degree;      // scale degree to be played
        float after;        // how many bars to start playing after
        float duration;     // for how many bars to play
        Config * config;    // optional configuration for the layer
    };

    Player(Looper & looper, unsigned bpm);

    void play(const Recipe recipes[], unsigned count);

private:
    Looper & _looper;
    unsigned _bpm;
};

} // midier

// "Player/Basic" - Plays basic recipe of layers serially
//
// This sketch shows how one create a recipe of layers and play them
// with a certain BPM using `Player`
//
// Setup needed to run the examples: https://github.com/levosos/Midier#setup
// No additional setup is required for this sketch
//
#include <Midier.h>

void setup()
{
    // if you are connecting using USB, set the baud rate that is configured in the serial-to-MIDI software
    // if you are connecting using a physical MIDI connector and cable, set it to 31250
    // if you're not sure what this is, or which baud rate to use: https://github.com/levosos/Midier#baud-rate
    Serial.begin(9600);
}

void loop()
{
    // a basic recipe of serial layers playing a musical cadence 1-4-5-1
    const midier::Player::Recipe recipes[] =
        {
            { .degree = 1, .after = 0, .duration = 1 },
            { .degree = 4, .after = 1, .duration = 1 },
            { .degree = 5, .after = 2, .duration = 1 },
            { .degree = 1, .after = 3, .duration = 1 },
        };

    // create a container for 1 layer as only a single layer will be played at a time
    midier::Layers<1> layers;

    // create a looper from the layers container
    midier::Looper looper(layers);

    // create a player from the looper to use a BPM of 60
    midier::Player player(looper, 60);

    // play the recipe
    player.play(recipes, sizeof(recipes) / sizeof(recipes[0]));
}

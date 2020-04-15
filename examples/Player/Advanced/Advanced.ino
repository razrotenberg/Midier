// "Player/Advanced" - Plays an advanced recipe of layers with different configurations simultaneously
//
// This sketch shows how one create a complex recipe of multiple layers to be played
// simultaneously with different layer configurations and with an offset using `Player`
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
    // create a container for 2 layers as 2 layers will be played at a time
    midier::Layers<2> layers;

    // create a looper from the layers container
    midier::Looper looper(layers);

    // create a player from the looper to use a BPM of 60
    midier::Player player(looper, 60);

    // play all the sections of the demo one after another
    playSectionA(player);
    playSectionB(player);
    playSectionC(player);
    playSectionD(player);

    // wait for 2 seconds before the next round
    delay(2000);
}

void playSectionA(midier::Player & player)
{
    midier::Config configA;
    configA.rhythm(midier::Rhythm::_3); // [*  *  *     ]

    // a basic 1-4-5-1 cadence
    const midier::Player::Recipe recipe[] =
        {
            { .degree = 1, .after = 0, .duration = 2, .config = &configA },
            { .degree = 4, .after = 2, .duration = 2, .config = &configA },
            { .degree = 5, .after = 4, .duration = 2, .config = &configA },
            { .degree = 1, .after = 6, .duration = 2, .config = &configA },
        };

    player.play(recipe, sizeof(recipe) / sizeof(recipe[0]));
}

void playSectionB(midier::Player & player)
{
    midier::Config configA;
    configA.rhythm(midier::Rhythm::_3); // [*  *  *     ]

    midier::Config configB;
    configB.octave(4); // an octave higher
    configB.rhythm(midier::Rhythm::_3); // [*  *  *     ]
    configB.style.perm(5); // 5-3-1

    // playing two layers at once with different style and in a different octave
    const midier::Player::Recipe recipe[] =
        {
            { .degree = 1, .after = 0, .duration = 2, .config = &configA },
            { .degree = 1, .after = 0, .duration = 2, .config = &configB },

            { .degree = 4, .after = 2, .duration = 2, .config = &configA },
            { .degree = 4, .after = 2, .duration = 2, .config = &configB },

            { .degree = 5, .after = 4, .duration = 2, .config = &configA },
            { .degree = 5, .after = 4, .duration = 2, .config = &configB },

            { .degree = 1, .after = 6, .duration = 2, .config = &configA },
            { .degree = 1, .after = 6, .duration = 2, .config = &configB },
        };

    player.play(recipe, sizeof(recipe) / sizeof(recipe[0]));
}

void playSectionC(midier::Player & player)
{
    midier::Config configA;
    configA.rhythm(midier::Rhythm::_3); // [*  *  *     ]

    midier::Config configC;
    configC.octave(4); // an octave higher
    configC.rhythm(midier::Rhythm::_7); // [*   *   *   ]
    configC.style.perm(5); // 5-3-1

    // playing two layers at once with different rhythms
    const midier::Player::Recipe recipe[] =
        {
            { .degree = 1, .after = 0, .duration = 2, .config = &configA },
            { .degree = 1, .after = 0, .duration = 2, .config = &configC },

            { .degree = 4, .after = 2, .duration = 2, .config = &configA },
            { .degree = 4, .after = 2, .duration = 2, .config = &configC },

            { .degree = 5, .after = 4, .duration = 2, .config = &configA },
            { .degree = 5, .after = 4, .duration = 2, .config = &configC },

            { .degree = 1, .after = 6, .duration = 2, .config = &configA },
            { .degree = 1, .after = 6, .duration = 2, .config = &configC },
        };

    player.play(recipe, sizeof(recipe) / sizeof(recipe[0]));
}

void playSectionD(midier::Player & player)
{
    midier::Config configA;
    configA.rhythm(midier::Rhythm::_3); // [*  *  *     ]

    midier::Config configD;
    configD.octave(4); // an octave higher
    configD.rhythm(midier::Rhythm::_2); // [*  *  *  *  ]
    configD.style.perm(5); // 5-3-1

    // playing two layers with an offset
    const midier::Player::Recipe recipe[] =
        {
            { .degree = 1, .after = 0,   .duration = 2, .config = &configA },
            { .degree = 1, .after = 0.5, .duration = 2, .config = &configD },

            { .degree = 4, .after = 2,   .duration = 2, .config = &configA },
            { .degree = 4, .after = 2.5, .duration = 2, .config = &configD },

            { .degree = 5, .after = 4,   .duration = 2, .config = &configA },
            { .degree = 5, .after = 4.5, .duration = 2, .config = &configD },

            { .degree = 1, .after = 6,   .duration = 2, .config = &configA },
            { .degree = 1, .after = 6.5, .duration = 2, .config = &configD },
        };

    player.play(recipe, sizeof(recipe) / sizeof(recipe[0]));
}

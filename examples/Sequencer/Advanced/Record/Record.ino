// "Sequencer/Advanced/Record" - Recording and playbacking layers
//
// This example shows how one can record a few bars and then play more
// layers on top of them while in playback mode.
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
    // create a common configuration for the background layers
    const midier::Config backgroundConfig =
        {
            .note       = midier::Note::D,
            .accidental = midier::Accidental::Sharp,
            .octave     = 2,
            .mode       = midier::Mode::Aeolian,
            .rhythm     = midier::Rhythm::Eighth,
            .steps      = 4,
            .perm       = 23,
        };

    // create a container for four concurrent layers
    midier::Layers<4> layers;

    // create a sequencer
    midier::Sequencer sequencer(layers, backgroundConfig);

    // play the intro while in wander mode w/o recording
    sequencer.play(1, { .bars = 2 });
    sequencer.play(4, { .bars = 2 });
    sequencer.play(1, { .bars = 2 });
    sequencer.play(5, { .bars = 2 });

    // start recording
    sequencer.record();

    // play and record the back track
    sequencer.play(4, { .bars = 2 });
    sequencer.play(5, { .bars = 2 });
    sequencer.play(1, { .bars = 4 });

    // stop recording and start playing back all 8 bars
    sequencer.record();

    // play the lead role four times
    for (unsigned i = 0; i < 4; ++i)
    {
        // create a forked configuration for the lead role
        const midier::Config leadConfig = backgroundConfig
            .octave(3) // one octave higher
            .rhythm(midier::Rhythm::Sixteenth)
            .perm(5); // 1 7 5 3

        // we chose to play the same degrees
        sequencer.play(4, { .bars = 2 }, leadConfig);
        sequencer.play(5, { .bars = 2 }, leadConfig);
        sequencer.play(1, { .bars = 4 }, leadConfig);
    }
}

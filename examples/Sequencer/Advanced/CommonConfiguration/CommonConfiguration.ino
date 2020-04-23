// "Sequencer/Advanced/CommonConfiguration" - Changes the common configuration of a `Sequencer` in runtime
//
// This example shows how easy is to change the common configuration of a `Sequencer`
// while affecting all the layers that are being played at the moment.
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
    // create the initial common configuration
    const midier::Config config =
        {
            .note       = midier::Note::C,
            .accidental = midier::Accidental::Natural,
            .octave     = 3,
            .mode       = midier::Mode::Aeolian,
            .rhythm     = midier::Rhythm::Eighth,
            .steps      = 4,
            .perm       = 23, // descending; this is the last permutation in [0,4!=24)
        };

    // create a container for two concurrent layers
    midier::Layers<2> layers;

    // create a sequencer and specify the initial common configuration
    midier::Sequencer sequencer(layers, config);

    // start degree I
    midier::Sequencer::Handle I = sequencer.start(1);

    // run for 4.25 bars
    sequencer.run(4.25);

    // last note of degree I was played at 4.0 and the next one will be
    // played in 4.5 as we are playing eighth notes (one every half a bar)

    // start degree V
    midier::Sequencer::Handle V = sequencer.start(5);

    // run for 3.75 bars so in total there will be 8 bars played
    sequencer.run(3.75);

    // change the common configuration while the layers are still being played
    sequencer.config = config.note(midier::Note::G);

    // run for 8 more bars in the new configuration
    sequencer.run(8);
}

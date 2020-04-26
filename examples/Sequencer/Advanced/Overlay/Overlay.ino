// "Sequencer/Advanced/Overlay" - Recording additional layers over a recorded loop
//
// This example shows how layers can be recorded on top of an already
// recorded loop when in overlay mode.
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
    // create a common configuration
    const midier::Config configA =
        {
            .note       = midier::Note::E,
            .accidental = midier::Accidental::Natural,
            .octave     = 3,
            .mode       = midier::Mode::Ionian,
            .rhythm     = midier::Rhythm::One_And_A,
        };

    // create a container for eight concurrent layers
    midier::Layers<8> layers;

    // create a sequencer with a BPM of 80
    midier::Sequencer sequencer(layers, 80);

    // start recording
    sequencer.record();

    sequencer.play(1, { .bars = 1 }, configA);
    sequencer.run({ .bars = 1 });
    sequencer.play(5, { .bars = 1 }, configA);
    sequencer.run({ .bars = 1 });
    sequencer.play(6, { .bars = 1 }, configA);
    sequencer.run({ .bars = 1 });
    sequencer.play(4, { .bars = 1 }, configA);
    sequencer.run({ .bars = 1 });

    // stop recording and start playing back all 8 bars
    sequencer.record();

    const midier::Config configB = configA
        .octave(4)
        .rhythm(midier::Rhythm::Sixteenth)
        .steps(4);

    // play a temporary layer for four bars
    sequencer.play(7, { .bars = 4 }, configB);

    // pass four more layers for the recorded loop to fully finish
    sequencer.run({ .bars = 4 });

    // enter overlay mode and record on top of the recorded layer
    sequencer.record();

    // play and record new layers in between the already recorded ones
    sequencer.run({ . bars = 1 });
    sequencer.play(5, { .bars = 1 }, configB);
    sequencer.run({ . bars = 1 });
    sequencer.play(4, { .bars = 1 }, configB);
    sequencer.run({ . bars = 1 });
    sequencer.play(3, { .bars = 1 }, configB);
    sequencer.run({ . bars = 1 });
    sequencer.play(2, { .bars = 1 }, configB);

    // stop overlaying and go back to playback
    sequencer.record();

    // now keep playing the recorded loop
    while (true)
    {
        sequencer.run({ .bars = 8 });
    }
}

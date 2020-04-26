// "Sequencer/Basic/Canon" - Plays Pachelbel’s Canon in D major
//
// This example plays Pachelbel’s Canon in D major.
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
    midier::Config config =
        {
            .note       = midier::Note::D,
            .accidental = midier::Accidental::Natural,
            .octave     = 3,
            .mode       = midier::Mode::Ionian,
            .rhythm     = midier::Rhythm::Eighth,
            .steps      = 4,
        };

    // create a container for one layer as only a single layer will be played at a time
    midier::Layers<1> layers;

    // create a sequencer from the layers container
    midier::Sequencer sequencer(layers, config, 70);

    sequencer.play(1, { .bars = 2 });
    sequencer.play(5, { .bars = 2 });
    sequencer.play(6, { .bars = 2 });
    sequencer.play(3, { .bars = 2 });
    sequencer.play(4, { .bars = 2 });
    sequencer.play(1, { .bars = 2 });
    sequencer.play(4, { .bars = 2 });
    sequencer.play(5, { .bars = 2 });
}

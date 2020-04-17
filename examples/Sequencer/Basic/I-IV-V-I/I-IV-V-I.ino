// "Sequencer/Basic/I-IV-V-I" - Plays the popular chord progression I-IV-V-I
//
// This sketch shows how it's easy to program a chord progression with `midier::Sequencer`
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
    // create a container for one layer as only a single layer will be played at a time
    midier::Layers<1> layers;

    // create a sequencer from the layers container
    midier::Sequencer sequencer(layers);

    sequencer.play(1, { .bars = 1 });
    sequencer.play(4, { .bars = 1 });
    sequencer.play(5, { .bars = 1 });
    sequencer.play(1, { .bars = 1 });
}

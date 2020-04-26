// "Sequencer/Advanced/Assist" - Plays layers with different assistance modes
//
// This example demonstrates the different assistance options.
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
    // create a container for two layers
    midier::Layers<2> layers;

    // create a sequencer
    midier::Sequencer sequencer(layers);

    // record a basic loop of a single layer for four bars
    sequencer.record();
    sequencer.play(1, { .bars = 4 });
    sequencer.record();

    // play without assistance
    // also by default there is no assistance
    // the layers will be played just as we play them, too close to each other
    play(sequencer, midier::Sequencer::Assist::No);

    // play with assist mode set to `Half`
    // the layers wil not be played at the same time but will be overlapped
    play(sequencer, midier::Sequencer::Assist::Half);

    // play with assist mode set to `Full`
    // the layers wil not be played at the very same time
    play(sequencer, midier::Sequencer::Assist::Full);
}

void play(midier::Sequencer & sequencer, midier::Sequencer::Assist assist)
{
    constexpr float delay = 0.06;

    // set the sequencer assist mode
    sequencer.assist = assist;

    // wait for a very short amount of time
    sequencer.run(delay);

    // play a new layer for four bars
    sequencer.play(5, { .bars = 4 });

    // play the remaining time to complete four bars
    sequencer.run(4 - delay);
}

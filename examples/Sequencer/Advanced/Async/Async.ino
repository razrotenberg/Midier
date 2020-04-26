// "Sequencer/Advanced/Async" - Changes the sequencer BPM in runtime
//
// This example shows how to work with Midier's asynchronous interface.
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
    midier::Layers<1> layers;
    midier::Sequencer sequencer(layers);

    // start a single layer and play it for eternity
    sequencer.start(1);

    // run asynchronously and change the BPM
    int delta = 2;
    unsigned long long timestamp = millis();

    while (true)
    {
        sequencer.click(midier::Sequencer::Run::Async);

        if (millis() - timestamp >= 100)
        {
            timestamp = millis();

            if (sequencer.bpm >= 230 || sequencer.bpm <= 20)
            {
                // reached the maximum or minimum BPM
                // start descending or ascending, respectively
                delta *= -1;
            }

            sequencer.bpm += delta;
        }
    }
}

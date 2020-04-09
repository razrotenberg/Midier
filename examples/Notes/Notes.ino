// "Notes" - Plays the basic notes sequentially
//
// Running this sketch and hearing musical notes from your computer
// or connected device will verify your setup is working.
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
    // have a list of the basic notes
    const midier::Note notes[] = {
        midier::Note::C,
        midier::Note::D,
        midier::Note::E,
        midier::Note::F,
        midier::Note::G,
        midier::Note::A,
        midier::Note::B,
    };

    // iterate over the notes
    for (auto note : notes)
    {
        // play the note
        midier::midi::play(note);

        // sleep for one second
        delay(1000);
    }

    // EXERCISE: play notes on different octaves
}

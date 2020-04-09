// "Intervals" - Builds and plays major and minor chords from root notes
//
// Running this sketch will show you how easily you can use musical intervals
// to generate and play patterns of notes like major and minor chords.
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
    // have a list of the basic notes as the root notes
    const midier::Note notes[] = {
        midier::Note::C,
        midier::Note::D,
        midier::Note::E,
        midier::Note::F,
        midier::Note::G,
        midier::Note::A,
        midier::Note::B,
    };

    // iterate over all root notes
    for (auto note : notes)
    {
        // play the major chord
        playMajorChord(note);

        // sleep for a short time
        delay(600);

        // play the minor chord
        playMinorChord(note);

        // sleep for a bit longer time
        delay(1500);
    }
}

void playNote(midier::Note note)
{
    // play the note
    midier::midi::play(note);

    // sleep for a very short time
    delay(400);
}

void playMajorChord(midier::Note root)
{
    // a major chord has a root, major third, and perfect fifth

    midier::Note third = root + midier::Interval::M3;
    midier::Note fifth = root + midier::Interval::P5;

    playNote(root);
    playNote(third);
    playNote(fifth);
}

void playMinorChord(midier::Note root)
{
    // a minor chord has a root, minor third, and perfect fifth

    midier::Note third = root + midier::Interval::m3;
    midier::Note fifth = root + midier::Interval::P5;

    playNote(root);
    playNote(third);
    playNote(fifth);
}

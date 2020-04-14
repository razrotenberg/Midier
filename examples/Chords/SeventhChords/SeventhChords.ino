// "SeventhChords" - Plays all seventh chords of root notes
//
// Running this sketch will show you how easily you can play seventh chords.
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

    // iterate over all the root notes
    for (auto root : notes)
    {
        // have a list of all seventh chord qualities
        const midier::Quality qualities[] = {
            midier::Quality::m7b5,
            midier::Quality::m7,
            midier::Quality::dom7,
            midier::Quality::maj7,
            midier::Quality::aug7,
        };

        // iterate over all the qualities
        for (auto quality : qualities)
        {
            // a list of all seventh chord degrees
            const midier::Degree degrees[] = { 1, 3, 5, 7 };

            // iterate over all the degrees
            for (auto degree : degrees)
            {
                // find out the interval to be added to the root note for this degree and quality
                midier::Interval interval = midier::triad::interval(quality, degree);

                // calculate the note of this degree
                midier::Note note = root + interval;

                // play the note
                playNote(note);
            }

            // sleep for some time
            delay(1000);
        }

        // sleep for some longer time
        delay(2000);
    }
}

void playNote(midier::Note note)
{
    // play the note
    midier::midi::play(note);

    // sleep for a short time
    delay(400);
}

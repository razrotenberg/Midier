// "Scales" - Plays all triads in a scale
//
// Running this sketch will show you how easily you can use iterate over
// any of the common diatonic scales from any root note and play it entirely.
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
    // the root note of the scale
    const midier::Note scaleRoot = midier::Note::G;

    // we are playing ionian mode which is the major scale
    // if you are not familiar with modes, just know that "ionian" is the major scale and "aeolian" is the minor scale
    const midier::Mode mode = midier::Mode::Ionian;

    // iterate over all the degrees in the scale
    // all the diatonic scales are heptatonic and have seven pitches per octave
    // this means that the eighth degree is an octave above the first one
    for (midier::Degree scaleDegree = 1; scaleDegree <= 8; ++scaleDegree)
    {
        // determine the chord quality of this scale degree
        const midier::Quality chordQuality = midier::scale::quality(mode, scaleDegree);

        // determine the interval from the root of the scale to the chord of this scale degree
        const midier::Interval interval = midier::scale::interval(mode, scaleDegree);

        // calculate the root note of the chord of this scale degree
        const midier::Note chordRoot = scaleRoot + interval;

        // play the triad of this scale degree
        playTriad(chordRoot, chordQuality);
    }
}

void playTriad(midier::Note chordRoot, midier::Quality chordQuality)
{
    // a list of all triad degrees
    const midier::Degree chordDegrees[] = { 1, 3, 5 };

    // iterate over all the degrees of the triad
    for (midier::Degree chordDegree : chordDegrees)
    {
        // find out the interval to be added to the root note for this chord degree and chord quality
        const midier::Interval interval = midier::triad::interval(chordQuality, chordDegree);

        // calculate the note of this chord degree
        const midier::Note note = chordRoot + interval;

        // play the note
        playNote(note);
    }

    // sleep for some time
    delay(600);
}

void playNote(midier::Note note)
{
    // play the note
    midier::midi::play(note);

    // sleep for a short time
    delay(400);
}

// "Sequencer/Basic/TwelveBarBlues" - Plays the 12-blues chord progression
//
// This sketch shows how it's easy to program even more complex chord
// progressions with custom configuration with `midier::Sequencer`
//
// Playing the following progression with seventh chords over A major scale:
//    I  I  I  I
//    IV IV I  I
//    V  IV I  V
//
// Setup needed to run the examples: https://github.com/levosos/Midier#setup
// No additional setup is required for this sketch
//
#include <Midier.h>

// bars in midier are in time signature 1:4
// we play each bar 4 times to make it 4:4
#define COMMON_TIME(x) (4 * (x))

void setup()
{
    // if you are connecting using USB, set the baud rate that is configured in the serial-to-MIDI software
    // if you are connecting using a physical MIDI connector and cable, set it to 31250
    // if you're not sure what this is, or which baud rate to use: https://github.com/levosos/Midier#baud-rate
    Serial.begin(9600);
}

void loop()
{
    // create a configuration:
    //   A major scale
    //   Shuffle rhythm
    //   Use seventh chords
    //
    const midier::Config config =
        {
            .note       = midier::Note::A,
            .accidental = midier::Accidental::Natural,
            .octave     = 3,
            .mode       = midier::Mode::Ionian,
            .rhythm     = midier::Rhythm::Swung_Triplet,
            .steps      = 4, // play the 7th degree as well
            .perm       = 0,
            .looped     = false,
        };

    // create a container for one layer as only a single
    // layer will be played at a time and use the custom configuration
    midier::Layers<1> layers;

    // create a sequencer from the layers container and specify BPM of 120
    midier::Sequencer sequencer(layers, config, 120);

    // play the first section
    sequencer.play(1, { .bars = COMMON_TIME(4) });

    // play the second section
    sequencer.play(4, { .bars = COMMON_TIME(2) });
    sequencer.play(1, { .bars = COMMON_TIME(2) });

    // play the third section
    sequencer.play(5, { .bars = COMMON_TIME(1) });
    sequencer.play(4, { .bars = COMMON_TIME(1) });
    sequencer.play(1, { .bars = COMMON_TIME(1) });
    sequencer.play(5, { .bars = COMMON_TIME(1) });
}

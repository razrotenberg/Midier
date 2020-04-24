// "Sequencer/Advanced/LayerConfiguration" - Detaching and re-attaching a layer from the common configuration
//
// This example shows how layers can detach from the common configuration and have a
// different configuration and can be re-attached to the common configuration at any time.
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
    // create the common configuration
    const midier::Config config =
        {
            .note       = midier::Note::G,
            .accidental = midier::Accidental::Natural,
            .octave     = 3,
            .mode       = midier::Mode::Ionian,
            .rhythm     = midier::Rhythm::Swung_Sextuplet,
            .steps      = 4,
        };

    // create a container for a single layer
    midier::Layers<1> layers;

    // create a sequencer and specify the initial common configuration
    midier::Sequencer sequencer(layers, config);

    // start playing an arpeggio
    midier::Sequencer::Handle handle = sequencer.start(1);

    // run for four bars
    sequencer.run(4);

    // change the common configuration
    sequencer.config = config.octave(4);

    // run for another four bars and hear that the layer was affected by the
    // change of the common configuration and is played an octave higher
    sequencer.run(4);

    // detach from the common configuration
    *handle.config = midier::Config
        {
            .note       = midier::Note::D,
            .accidental = midier::Accidental::Sharp,
            .octave     = 3,
            .mode       = midier::Mode::Aeolian,
            .rhythm     = midier::Rhythm::Sixteenth,
            .steps      = 4,
        };

    // run for two bars and hear the new layer configuration
    sequencer.run(2);

    // change the common configuration once again
    sequencer.config = midier::Config
        {
            .note       = midier::Note::F,
            .accidental = midier::Accidental::Natural,
            .octave     = 5,
        };

    // run for another two bars and hear that the layer was not affected by the
    // change of the common configuration this time, as it was detached before
    sequencer.run(2);

    // re-attach to the common configuration
    *handle.config = &sequencer.config;

    // run for four bars and hear that the layer is played in the new common configuration
    sequencer.run(4);
}

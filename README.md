# Midier

Midier is a library written in C++ to play, record, loop and program MIDI notes, arpeggios and sequences on Arduino

## Table of Contents

* [What Is Midier?](#what-is-midier)
* [Listen to Examples](#listen-to-examples)
* [How to Use Midier?](#how-to-use-midier)
    * [Playing Notes](#playing-notes)
    * [Intervals](#intervals)
    * [Chord Qualities](#chord-qualities)
    * [Scales](#scales)
    * [Chord Progressions and Complex Sequences](#chord-progressions-and-complex-sequences)
    * [Configuration](#configuration)
    * [Recording and Looping](#recording-and-looping)
    * [Asynchronous Interface](#asynchronous-interface)
    * [Assistance](#assistance)
* [Setup](#setup)
* [Debugging](#debugging)
* [Classes](#classes)

## What Is Midier?

*Midier* is a library for Arduino, that enables creating, playing, recording, looping and programming MIDI notes, arpeggios, chord progressions and complex sequences easily.

## Listen to Examples

There are plenty of examples of *Midier* that require nothing but an Arduino and a USB cable that you can run.
Running the examples is super easy, and you will see how you can do cool stuff with *Midier* with only a few lines of code.

Listen to a few melodies created easily with *Midier*:

| Name | Description | &#127926; |
|------|-------------|-----------|
| [Canon](examples/Sequencer/Basic/Canon/Canon.ino) | Plays Pachelbel’s Canon in D major | [Listen](https://levosos.github.io/Midier/examples/Sequencer/Basic/Canon/Canon.mp3) |
| [I-IV-V-I](examples/Sequencer/Basic/I-IV-V-I/I-IV-V-I.ino) | Plays the common I-IV-V-I chord progression | [Listen](https://levosos.github.io/Midier/examples/Sequencer/Basic/I-IV-V-I/I-IV-V-I.mp3) |
| [12-Bar Blues](examples/Sequencer/Basic/TwelveBarBlues/TwelveBarBlues.ino) | Plays a 12-bar blues chord progression | [Listen](https://levosos.github.io/Midier/examples/Sequencer/Basic/TwelveBarBlues/TwelveBarBlues.mp3) |

See how simple it is to implement I-IV-V-I for example:

```
midier::Layers<1> layers;
midier::Sequencer sequencer(layers);

sequencer.play(1, { .bars = 1 });
sequencer.play(4, { .bars = 1 });
sequencer.play(5, { .bars = 1 });
sequencer.play(1, { .bars = 1 });
```

Here is a table of all examples provided in addition to the three melodies above.
The examples are documented and show how to use all of the features of *Midier*.
Each example has a purpose and shows some feature of *Midier*.
The table is ordered in the recommended steps to master *Midier*:

| Name | Description | &#127926; |
|------|-------------|-----------|
| [Notes](examples/Notes/Notes.ino) | Plays the basic notes sequentially | [Listen](https://levosos.github.io/Midier/examples/Notes/Notes.mp3) |
| [Intervals](examples/Intervals/Intervals.ino) | Plays major and minor chords of root notes | [Listen](https://levosos.github.io/Midier/examples/Intervals/Intervals.mp3) |
| [Triads](examples/Chords/Triads/Triads.ino) | Plays all triad qualities of root notes | [Listen](https://levosos.github.io/Midier/examples/Chords/Triads/Triads.mp3) |
| [Seventh Chords](examples/Chords/SeventhChords/SeventhChords.ino) | Plays all seventh chords of root notes | [Listen](https://levosos.github.io/Midier/examples/Chords/SeventhChords/SeventhChords.mp3) |
| [Scales](examples/Scales/Scales.ino) | Plays all triads in a scale | [Listen](https://levosos.github.io/Midier/examples/Scales/Scales.mp3) |
| [Common Configuration](examples/Sequencer/Advanced/CommonConfiguration/CommonConfiguration.ino) | Changes the common configuration of a `Sequencer` in runtime | [Listen](https://levosos.github.io/Midier/examples/Sequencer/Advanced/CommonConfiguration/CommonConfiguration.mp3) |
| [Layer Configuration](examples/Sequencer/Advanced/LayerConfiguration/LayerConfiguration.ino) | Detaching and re-attaching a layer from the common configuration | [Listen](https://levosos.github.io/Midier/examples/Sequencer/Advanced/LayerConfiguration/LayerConfiguration.mp3) |
| [Record](examples/Sequencer/Advanced/Record/Record.ino) | Recording and playbacking layers | [Listen](https://levosos.github.io/Midier/examples/Sequencer/Advanced/Record/Record.mp3) |
| [Overlay](examples/Sequencer/Advanced/Overlay/Overlay.ino) | Recording additional layers over a recorded loop | [Listen](https://levosos.github.io/Midier/examples/Sequencer/Advanced/Overlay/Overlay.mp3) |
| [Assist](examples/Sequencer/Advanced/Assist/Assist.ino) | Plays layers with different assistance modes | [Listen](https://levosos.github.io/Midier/examples/Sequencer/Advanced/Assist/Assist.mp3) |
| [Async](examples/Sequencer/Advanced/Async/Async.ino) | Uses the async API and changes the sequencer BPM in runtime | [Listen](https://levosos.github.io/Midier/examples/Sequencer/Advanced/Async/Async.mp3) |

Read the [Setup](#Setup) section below for more information of the setup required to run the examples.

## How to Use Midier?

### Playing Notes

At its core, *Midier* enables playing MIDI notes.

```
midier::midi::play(midier::Note::G);
```

> Check out the [Notes](examples/Notes/Notes.ino) example that plays all the notes

### Intervals

*Midier* allows you to intuitively use musical intervals to calculate note values. Thus, *Midier* provides an easy API to easily build and play arpeggios and chords.

```
// the notes in a C major chord
midier::Note root = midier::Note::C;
midier::Note third = root + midier::Interval::M3;
midier::Note fifth = root + midier::Interval::P5;

// play a C major arpeggio
midier::midi::play(root);
midier::midi::play(third);
midier::midi::play(fifth);
```

> Check out the [Intervals](examples/Intervals/Intervals.ino) example that plays all the major and minor chords

### Chord Qualities

*Midier* provides an even easier way to generate triads (3-note chords) and seventh-chords (4-note chords) by querying the intervals of common chord qualities.

All supported qualities are listed in [quality.h](src/quality/quality.h).

> Check out examples [Triads](examples/Chords/Triads/Triads.ino) and [SeventhChords](examples/Chords/SeventhChords/SeventhChords.ino) that play all qualities of triads and seventh-chords

### Scales

*Midier* provides an easy way to play scales. It supports the seven modes of the diatonic scale, which are listed in [mode.h](src/mode/mode.h).

You can query the quality of any scale degree of any of the seven common modes, as well as the interval of the scale degree from the scale root. Using this two pieces of information - the interval and the quality - you can easily play its chord or arpeggio.

```
midier::Mode mode = midier::Mode::Ionian;
int degree = 5;

midier::Quality quality = midier::scale::quality(mode, degree);
midier::Interval interval = midier::scale::interval(mode, degree);
```

> Check out the [Scales](examples/Scales/Scales.ino) example that plays all degrees in the G major scale

### Chord Progressions and Complex Sequences

In addition to providing an interface for playing notes, and using musical intervals and scales to generate chords and arpeggios, *Midier* also provides an even easier to use interface for creating and playing chord progressions and complex sequences. This interface is `Sequencer` and declared in [sequencer.h](src/sequencer/sequencer.h).

In many ways, while coming up with the idea of *Midier* and its uses, the `Sequencer` interface was the one meant to be exposed to the user, while all the above are essential for implementing it, and may also not be used by the user at all.

There are both synchronous and asynchronous interfaces to use `Sequencer`.
It's easy to explore the capabilities of *Midier* using the synchronous interface, with regular sequential lines of code (as in most of the examples).
The asynchronous interface may be used by more complex Arduino projects that also have other things to do (such as handling user I/O).

A `Sequencer` object provides an easy interface to play arpeggios of different scale degrees.
An arpeggio of a scale degree that is being played in a `Sequencer` is called a `Layer`.
`Layer`s can be played at any time, in any order, and for any duration of time.
`Layer`s can be played sequentially one after another, simultaneously at the same time, or in parallel with some overlap.
This enables to easily play both simple and complex sequences.

Playing an arpeggiated chord progression is as easy as the next few lines of code:

```
// create a container for the layers
midier::Layers<1> layers;

// create the sequencer
midier::Sequencer sequencer(layers);

// play I-IV-V-I chord progression
sequencer.play(1, { .bars = 1 });
sequencer.play(4, { .bars = 1 });
sequencer.play(5, { .bars = 1 });
sequencer.play(1, { .bars = 1 });
```

> Check out examples [Canon](examples/Sequencer/Basic/Canon/Canon.ino), [I-IV-V-I](examples/Sequencer/Basic/I-IV-V-I/I-IV-V-I.ino) and [TwelveBarBlues](examples/Sequencer/Basic/TwelveBarBlues/TwelveBarBlues.ino) that play common chord progressions

### Configuration

You can configure almost any aspect of an arpeggio with *Midier*.
Here's an example of a `Config` object, which is declared in [config.h](src/config/config.h):

```
midier::Config config =
    {
        .note = midier::Note::G,
        .accidental = midier::Accidental::Sharp,
        .octave = 3,
        .mode = midier::Mode::Aeolian,
        .rhythm = midier::Rhythm::Triplet,
        .steps = 4,
        .perm = 17,
        .looped = true,
    };
```

#### Fields

##### Root

The root note of the scale is specified by `.note`, `.accidental`, and `.octave`.
In the example above the arpeggio will be in a scale starting in G# in octave 3.

##### Mode

`Layer`s are scale degrees being played as arpeggios. The field `.mode` specifies the scale which the arpeggio is in.
In the example above, the arpeggio will be in a G# minor scale (aeolian mode).
Currently, *Midier* supports the seven modes of the diatonic scale:
- Ionian
- Dorian
- Phrygian
- Lydian
- Mixolydian
- Aeolian
- Locrian

##### Rhythm

An arpeggio is a sequence of notes. The field `.rhythm` specifies how these notes should be played, rhythmically.
Declared and documented in [rhythm.h](src/rhythm/rhythm.h), *Midier* supports several rhythms, in both straight and swing feels:

| Index | Name            | Pattern                                                                                          | Time                      |
|-------|-----------------|--------------------------------------------------------------------------------------------------|---------------------------|
| 0     | Quarter         | &#x2587;&#x2581;&#x2581;&#x2581;&#x2581;&#x2581;&#x2581;&#x2581;&#x2581;&#x2581;&#x2581;&#x2581; | 1/4                       |
| 1     | Eighth          | &#x2587;&#x2581;&#x2581;&#x2581;&#x2581;&#x2581;&#x2587;&#x2581;&#x2581;&#x2581;&#x2581;&#x2581; | 1/8 1/8                   |
| 2     | Sixteenth       | &#x2587;&#x2581;&#x2581;&#x2587;&#x2581;&#x2581;&#x2587;&#x2581;&#x2581;&#x2587;&#x2581;&#x2581; | 1/16 1/16 1/16 1/16       |
| 3     | 1-e-and         | &#x2587;&#x2581;&#x2581;&#x2587;&#x2581;&#x2581;&#x2587;&#x2581;&#x2581;&#x2581;&#x2581;&#x2581; | 1/16 1/16 1/8             |
| 4     | 1-and-a         | &#x2587;&#x2581;&#x2581;&#x2581;&#x2581;&#x2581;&#x2587;&#x2581;&#x2581;&#x2587;&#x2581;&#x2581; | 1/8 1/16 1/16             |
| 5     | 1-e-a           | &#x2587;&#x2581;&#x2581;&#x2587;&#x2581;&#x2581;&#x2581;&#x2581;&#x2581;&#x2587;&#x2581;&#x2581; | 1/16 1/8 1/16             |
| 6     | e-and-a         | &#x2581;&#x2581;&#x2581;&#x2587;&#x2581;&#x2581;&#x2587;&#x2581;&#x2581;&#x2587;&#x2581;&#x2581; | 1/16-rest 1/16 1/16 1/16  |
| 7     | Triplet         | &#x2587;&#x2581;&#x2581;&#x2581;&#x2587;&#x2581;&#x2581;&#x2581;&#x2587;&#x2581;&#x2581;&#x2581; | 1/8 note triplet          |
| 8     | Swung Triplet   | &#x2587;&#x2581;&#x2581;&#x2581;&#x2581;&#x2581;&#x2581;&#x2581;&#x2587;&#x2581;&#x2581;&#x2581; | 1/8 note swung triplet    |
| 9     | Two Bar Swing   | &#x2587;&#x2581;&#x2581;&#x2581;&#x2581;&#x2581;&#x2581;&#x2581;&#x2583;&#x2581;&#x2581;&#x2581; | 2-bar 1/8 note swing      |
| 10    | Sextuplet       | &#x2587;&#x2581;&#x2587;&#x2581;&#x2587;&#x2581;&#x2587;&#x2581;&#x2587;&#x2581;&#x2587;&#x2581; | 1/16 note sextuplet       |
| 11    | Swung Sextuplet | &#x2587;&#x2581;&#x2581;&#x2581;&#x2587;&#x2581;&#x2587;&#x2581;&#x2581;&#x2581;&#x2587;&#x2581; | 1/16 note swung sextuplet |

##### Steps

The field `.steps` specifies the number of notes that should be played in the arpeggio. This could be 3, 4, 5 or 6.
Playing 3 steps means playing chord degrees I, III and V, which is a triad.
Playing 4 steps means to also play degree VII, making the arpeggio a seventh chord.
Playing 5 and 6 steps means to play degrees I and III an octave higher (counted as degrees 8 and 10 respectively).

##### Permutation

The field `.perm` specifies the order in which the notes of the arpeggio should be played.
The maximum possible value of `perm` depends on the number of steps of the arpeggio, and is the factorial of it.
For 3 steps, there are 6 permutations, for 4 steps 24, for 5 steps 120, and for 6 steps there are 720.
*Midier* uses an algoritm, which is fully documented in [style.cpp](src/style/style.cpp), to generate any possible permutation of the arpeggio notes.
*Midier* also provides an interface to get a string describing the permutation by its index, by calling `midier::style::description()`.

##### Looped

If field `.looped` is set to `true`, all notes of the arpeggio (with any number of steps) are played in reverse order after being played normally.
This results in an infinite loop of notes being played ascending and descending.
Any configuration of number of steps and the permutation, can also be played `looped`.
This doubles the amount of possible configurations of an arpeggio in *Midier*.

#### Common Configuration

By default, all `Layer`s that are being played using the same `Sequencer` share a common configuration.
This is possible because `Sequencer` has a `Config` member, which is pointed by all `Layer`s by default.
Thus, changing the configuration of a `Sequencer` will immediately and automatically affect all current `Layer`s and all the next ones as well.

> Check out the [CommonConfiguration](examples/Sequencer/Advanced/CommonConfiguration/CommonConfiguration.ino) example that modifies the common configuration while playing a sequence

#### Layer Configuration

Every `Layer` can be detached from the common configuration and be configured differently.
This enables you play complex sequences of layers from different scales, with different number of steps and permutation, and in a different rhythm.

For example, you can play a `Layer` in C major scale playing triads in eighth note triplets, while simultaneously playing a `Layer` in G minor scale playing seventh-chords in sixteenth notes.

`Layer`s that were detached from the common configuration can be reattached to it, and vice versa.
This let's you enjoy the maximum freedom of experimenting with a layer configuration while always being able to go back and use the common configuration.

> Check out the [LayerConfiguration](examples/Sequencer/Advanced/LayerConfiguration/LayerConfiguration.ino) example that detaches a layer from the common configuration and then re-attaches it

### Recording and Looping

In addition to playing MIDI sequences, *Midier* also supports recording and looping sequences.

#### Wander

By default, a `Sequencer` is in *wander* state.
This means that layers start playing on a call to `start()`, are being played for some time, and then stop playing on a call to `stop()`.

#### Record

A `Sequencer` object can also record what is being played.
While recording, every starting and stopping of any layer will be remembered.
A `Sequencer` records full bars, and any number of bars can be recorded up to a certain limit, which is 48 (`Time::Bars`) currently.

A call to `record()` on a `Sequencer` which is in *wander* state will eventually start recording.
If there are layers being played on the call to `record()`, the `Sequencer` will enter *record* state and start recording immediately.
If there are no layers being played on the call to `record()`, the `Sequencer` will enter *pre-record* state.
Then, when the first layer starts playing, the `Sequencer` will enter *record* state and start recording.

#### Playback

While in *record* state, calling `record()` will cause the `Sequencer` to stop recording and enter *playback* state.
In *playback* state, the `Sequencer` will play the recorded bars repeatedly.
Every layer that was recorded when the `Sequencer` was previously in *record* state will play automatically in *playback* mode.

New layers can be played while in *playback* mode.
They will be played on top of the recorded loop and will not be recorded themselves.

#### Overlay

While in *playback* state, calling `record()` will cause the `Sequencer` to enter *overlay* state and start recording once again.
While in *overlay* state, new layers will be recorded on top of the already recorded loop.
In oppose to *record* state, no new bars are recorded in *playback* state, and the number of recorded will not change.
Calling `record` while in *overlay* state will change the state of a `Sequencer` to *playback*.

#### Summary

Calling `record()` on a `Sequencer` will change its state depending on the current state the `Sequencer` is in.
At first, a `Sequencer` is in *wander* state and calling `record()` will eventually change it to *record* (potentially via *pre-record*).
Calling `record()` will then determine the number of recorded bars and change the state to *playback*.
Then, calling `record()` will toggle between *overlay* and *playback* states.

At any time, a call to `wander()` will set a `Sequencer` to *wander* state and will cause it to immediately stop recording and playing back any layers.
Here's a summary of the potential changes of state of a `Sequencer`:

*wander* &#x2192; *pre-record* &#x2192; *record* &#x2192; *playback* &#x2192; *overlay* &#x2192; *playback* &#x2192; ... &#x2192; *wander*

> Check out examples [Record](examples/Sequencer/Advanced/Record/Record.ino) and [overlay](examples/Sequencer/Advanced/overlay/overlay.ino) that record a few bars and playback them

### Asynchronous Interface

*Midier* provides both synchronous and asynchronous interfaces for `Sequencer::click()`.
You can choose whether to use the synchronous or asynchronous interfaces by specifying `Run::Sync` and `Run::Async` respectively on a call to `Sequencer::click()`.

The synchronous API may be easier to use, and is good for simple and straight-forward uses, such as most of the examples of *Midier*.
When the method is called synchronously, it will not return until the click will actually happen.
When the click happens depends on the BPM of the `Sequencer`, and the time the previous click took place.
A certain amount of time has to pass between clicks in order to satisfy the specified BPM.

This asynchronous API is useful when you have more things you need to do and you can't wait for the click to actually happen.
This could happen for example if you handle user I/O operations such as button clicks.
I/O operations should be handled immediately, and waiting for the synchronous methods will create latency and bad feedback for the user.

Using the asynchronous API will cause `click()` to return immediately.
The click will take place on a call to `click()` if enough time has passed since the precious click.
In case not enough time has passed since the previous click, `click()` will do nothing and return immediately.

This let's you call `click()` asynchronously from your `loop()` method while still doing more tasks and without creating latency.

> Check out the [Async](examples/Sequencer/Advanced/Async/Async.ino) example that uses the asynchronous interface and changes the sequencer BPM to demonstrate an interactive task

### Assistance

The assistance mode in *Midier* determines when a new layer will actually starts.

By default, assistance is set to `Assist::No`. This means new layers will start immediately at the next click after the call to `start()`.
This may not always be ideal, as it may be pretty hard for humans to start layers on correct timing (for example by clicking a button).

*Midier* supports two other assistance modes to help with that, and cause new layers to be delayed just by a bit to be synchronized with previous layers.

`Assist::Full` specifies that layers should be synchronized with the full rate of the current rhythm.
For example, if the current rhythm is in rate of eighth notes, then new layers will start on exact eighth notes.

`Assist::Half` specifies that layers should be synchronized with the full rate of the current rhythm or the exact half of it.
For example, if the current rhythm is in rate of eighth notes, then new layers will start on exact eighth notes, or on exact sixteenth notes.

> Check out the [Assist](examples/Sequencer/Advanced/Assist/Assist.ino) example that demonstrates the different assistance modes

## Setup

*Midier* sends MIDI commands over the Arduino's serial connection. These MIDI commands can be the input to any device that supports MIDI as input.
A computer software can accept MIDI commands as well. Therefore, the computer can be the device we connect the Arduino to.

### Connection

There are two main methods to connect your Arduino to any device supporting MIDI - over USB cable or over MIDI cable.
In case you are using a computer software, both methods are suitable for you.
In case you are connecting to a physical MIDI device, you should use a physical MIDI connector and a MIDI cable.

#### USB
Connect the Arduino to your computer via USB, and use a software to convert the serial connection into a MIDI device.
A recommended software is [Hairless MIDI to Serial Bridge](http://projectgus.github.io/hairless-midiserial/) but you can choose your preferred software.

Note that you cannot upload software to the Arduino while it is connected to the serial-to-MIDI converter software, so make sure to unselect the Arduino serial port in the software before every time you upload anything to the Arduino.

#### MIDI
Connect a physical MIDI connector (5-pin DIN) to your Arduino.
There is an [official tutorial](https://www.arduino.cc/en/tutorial/midi) on how to do it.
Then, you can either connect it directly to any device that has a MIDI-in plug using a MIDI cable, or use a MIDI-to-USB cable to connect it to your computer.

Note that although it may be a bit misleading, but you will want to connect the MIDI-in cable to your Arduino and not the MIDI-out.
You can remember it because the device you are connecting to will receive *input* MIDI communication from this connection.

### Baud Rate

The connection method you chose dictates the baud rate you need to set your Arduino serial connection to.
MIDI protocol uses baud rate of 31250. Therefore, if you are connecting to a physical MIDI device directly using a MIDI cable, or to your computer using a MIDI-to-USB cable, make sure to set the baud rate to 31250.
If you are connecting using USB, make sure to set the same baud rate both in the Arduino sketch and in the software that converts the serial connection into a MIDI device on your computer.
You may find that the software does not support baud rate 31250, and you will have to pick another value. I use baud rate of 9600 when connected over USB.

In the Arduino sketch, set the serial connection baud rate by calling `Serial.begin()` and passing the baud rate (9600 for example) in your `setup()` method.

### MIDI Device

As said, *Midier* only sends MIDI commands. This means that there needs to be something that converts these MIDI commands into actual sounds.

If you are connecting your Arduino to a device that has a MIDI-in plug, such as a synthesizer, the device will generate sound from the MIDI commands.
If you are connecting your Arduino to a computer, you should use a software to convert these MIDI commands into sound.
Many DAW software can do that. For example: Ableton, Cubase, Logic Pro, GarageBand, Pro Tools, [LMMS](https://lmms.io/), and many more.

## Debugging

Debug mode is supported in *Midier*.

In debug mode, the library writes debug traces to the serial connection and MIDI notes are not being played. These traces are visible in the Arduino Serial Monitor.

To enable debug mode, uncomment line #3 in [debug.h](src/debug/debug.h). This declares a preprocessor macro called `DEBUG` which is respected in the macros `TRACE_*()`.

Strings should pass to `TRACE_*()` macros wrapped with the macro `F()` (e.g. `TRACE_1(F("Hello, world"))`) to place the strings in the program memory (flash) instead of in the RAM.

## Classes

### Interval

An `enum class` that represents a musical interval.

The `enum` values are the corresponding number of semitones.
In runtime, we might pass `Interval` values that are not declared in the header file. e.g: 13 (*m9*), 19 (*P12*).

`Interval`s can be added and results in another `Interval`.
Adding `Intervals` is done transparently using C++ `operator+()`. For example:
```
Interval p5 = Interval::m3 + Interval::M3; // this will result in 7 (Interval::P5)
```

### Note

An `enum class` that represents a musical note.

The `enum` values are the corresponding number of semitones from the note 'C'.
This is to easily convert a note to its respective MIDI note number.

In runtime, we might pass `Note` values that are not declared in the header file. e.g: 12 (*C*), 20 (*G#*).

Adding `Note` with an `Interval` results in a `Note`.
This is done transparently using C++ `operator+()`. For example:
```
Note g = Note::C + Interval::P5; // this will result in 7 (Note::G)
```

### Accidental

An `enum class` that represents an accidental that can be applied to a note.
Supported accidentals are flat (♭), natural (♮), and sharp (♯).

The `enum` values are the corresponding number of semitones to raise or lower the note with.

Adding `Note` with an `Accidental` results in a `Note`.
This is done transparently using C++ `operator+()`. For example:
```
Note a_sharp = Note::A + Accidental::Sharp; // this will result in 10 (A#)
```

### Quality

An `enum class` that represents a chord quality.

*Midier* supports two types of chord qualities:
- Triads (3 notes)
- Seventh chords (4 notes)

Currently, qualities in *Midier* are used only to retrieve the interval of a chord degree using `triad::interval()`.

Every triad (3-note) quality has a matching seventh-chord (4-note) quality with the first three notes (chord degrees 1, 3, and 5) in common.
Therefore, in order to support both 7-th chords (4 notes) and triads (3 notes) qualities, we define triad qualities as an alias to a matching seventh-chord quality.

There's no need at the moment to be able to distinguish between 3-note qualities and their matching 4-note qualities by having different enum values.

# Midier

Midier is a library written in C++ to create, play, record and loop MIDI notes, arpeggios and sequences on Arduino

---

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
Many DAW software can do that. For example: Ableton, Cubase, Logic Pro, GarageBand, Pro Tools, and many more.

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

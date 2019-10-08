#include "midi.h"

#include <Arduino.h>

namespace midiate
{
namespace midi
{

namespace
{

void send(byte command, byte data1, byte data2, byte channel)
{
    Serial.write((command & 0xF0) | (channel & 0x0F));
    Serial.write(data1 & 0x7F);
    Serial.write(data2 & 0x7F);
}

} //

void play(const Pitch & pitch)
{
    constexpr auto channel = 0;

    send(0x90, pitch.note, 0x7F, channel); // note on on max velocity
    send(0x80, pitch.note, 0, channel); // note off
}

} // midi
} // midiate

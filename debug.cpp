#include "debug.h"

#ifdef DEBUG

#include "layer.h"
#include "time.h"

#include <Arduino.h>

namespace midiate
{
namespace debug
{

// Arduino Serial.print(...) methods

void print(const char x[])  { Serial.print(x); }
void print(char x)          { Serial.print(x); }
void print(unsigned char x) { Serial.print(x); }
void print(int x)           { Serial.print(x); }
void print(unsigned int x)  { Serial.print(x); }
void print(long x)          { Serial.print(x); }
void print(unsigned long x) { Serial.print(x); }

void println(const char x[])    { Serial.println(x); }
void println(char x)            { Serial.println(x); }
void println(unsigned char x)   { Serial.println(x); }
void println(int x)             { Serial.println(x); }
void println(unsigned int x)    { Serial.println(x); }
void println(long x)            { Serial.println(x); }
void println(unsigned long x)   { Serial.println(x); }
void println()                  { Serial.println();  }

// midiate specific

void print(const Time & time)
{
    print((int)time.bar);
    print('.');

    if (time.subdivision < 10)
    {
        print('0');
    }

    print((int)time.subdivision);
}

void print(const Layer & layer)
{
    print((int)layer.tag);
}

void prefix(const char file[], int line, const char function[])
{
    const char * pos = file;
    while (*pos != '\0')
    {
        if (*pos == '/')
        {
            file = pos + 1;
        }

        ++pos;
    }

    PRINT("[");
    PRINT(Time::now);
    PRINT("] ");
    PRINT(file);
    PRINT(':');
    PRINT(line);
    PRINT(" @ ");
    PRINT(function);
    PRINT("(): ");
}

} // debug
} // midiate

#endif

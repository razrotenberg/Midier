#include "debug.h"

#ifdef DEBUG

#include "../layer/layer.h"
#include "../time/time.h"

#if defined(ARDUINO)

namespace midier
{
namespace debug
{

// Arduino Serial.print(...) methods

void print(const char x[])                  { Serial.print(x); }
void print(char x)                          { Serial.print(x); }
void print(unsigned char x)                 { Serial.print(x); }
void print(int x)                           { Serial.print(x); }
void print(unsigned int x)                  { Serial.print(x); }
void print(long x)                          { Serial.print(x); }
void print(unsigned long x)                 { Serial.print(x); }
void print(float x)                         { Serial.print(x); }
void print(const __FlashStringHelper * x)   { Serial.print(x); }

void println(const char x[])                { Serial.println(x); }
void println(char x)                        { Serial.println(x); }
void println(unsigned char x)               { Serial.println(x); }
void println(int x)                         { Serial.println(x); }
void println(unsigned int x)                { Serial.println(x); }
void println(long x)                        { Serial.println(x); }
void println(unsigned long x)               { Serial.println(x); }
void println(float x)                       { Serial.println(x); }
void println(const __FlashStringHelper * x) { Serial.println(x); }
void println()                              { Serial.println();  }

// midier specific

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
    print(layer.id);
}

void prefix(const __FlashStringHelper * file, int line, const char function[])
{
    char const * const pos = strrchr_P((char const *)file, '/');
    if (pos != nullptr)
    {
        file = (const __FlashStringHelper *)(pos + 1);
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
} // midier

#elif defined(__EMSCRIPTEN__)

namespace midier
{
namespace debug
{

void prefix(char const * file, int line, const char function[])
{
    char const * pos = strrchr(file, '/');
    if (pos != nullptr)
    {
        file = pos + 1;
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

std::ostream & operator<<(std::ostream & os, const Time & time)
{
    os << (int)Time::now.bar;
    os << '.';

    if (Time::now.subdivision < 10)
    {
        os << '0';
    }

    os << (int)Time::now.subdivision;
    return os;
}

std::ostream & operator<<(std::ostream & os, const Layer & layer)
{
    return os << (unsigned)layer.id;
}

} // midier

#endif // ARDUINO / __EMSCRIPTEN__

#endif // DEBUG

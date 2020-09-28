#pragma once

// #define DEBUG // toggle debug mode

#ifdef DEBUG

namespace midier
{

struct Layer;
struct Time;

} // midier

#if defined(ARDUINO)

#include <Arduino.h>

namespace midier
{
namespace debug
{

// forward declaration of Arduino Serial.print(...) methods

void print(const char[]);
void print(char);
void print(unsigned char);
void print(int);
void print(unsigned int);
void print(long);
void print(unsigned long);
void print(float);
void print(const __FlashStringHelper *);

void println(const char[]);
void println(char);
void println(unsigned char);
void println(int);
void println(unsigned int);
void println(long);
void println(unsigned long);
void println(float);
void println(const __FlashStringHelper *);
void println();

// midier specific

void print(const Time &);
void print(const Layer &);

void prefix(const __FlashStringHelper * file, int line, const char function[]);

} // debug
} // midier

#define PRINT(x)        debug::print(x)
#define PRINTLN(x)      debug::println(x)
#define TRACE_START()   debug::prefix(F(__FILE__), __LINE__, __FUNCTION__)
#define TRACE_END() PRINTLN()

#elif defined(__EMSCRIPTEN__)

#include <iostream>

namespace midier
{

namespace debug
{

void prefix(char const * file, int line, const char function[]);

} // debug

std::ostream & operator<<(std::ostream &, const Time &);
std::ostream & operator<<(std::ostream &, const Layer &);

} // midier

#define F(x) (x)
#define PRINT(x) std::cout << (x)
#define TRACE_START() debug::prefix(__FILE__, __LINE__, __FUNCTION__)
#define TRACE_END() std::cout << std::endl

#endif // ARDUINO / __EMSCRIPTEN__

#define TRACE_1(x1)                         TRACE_START(); PRINT(x1);                                                                               TRACE_END()
#define TRACE_2(x1,x2)                      TRACE_START(); PRINT(x1); PRINT(x2);                                                                    TRACE_END()
#define TRACE_3(x1,x2,x3)                   TRACE_START(); PRINT(x1); PRINT(x2); PRINT(x3);                                                         TRACE_END()
#define TRACE_4(x1,x2,x3,x4)                TRACE_START(); PRINT(x1); PRINT(x2); PRINT(x3); PRINT(x4);                                              TRACE_END()
#define TRACE_5(x1,x2,x3,x4,x5)             TRACE_START(); PRINT(x1); PRINT(x2); PRINT(x3); PRINT(x4); PRINT(x5);                                   TRACE_END()
#define TRACE_6(x1,x2,x3,x4,x5,x6)          TRACE_START(); PRINT(x1); PRINT(x2); PRINT(x3); PRINT(x4); PRINT(x5); PRINT(x6);                        TRACE_END()
#define TRACE_7(x1,x2,x3,x4,x5,x6,x7)       TRACE_START(); PRINT(x1); PRINT(x2); PRINT(x3); PRINT(x4); PRINT(x5); PRINT(x6); PRINT(x7);             TRACE_END()
#define TRACE_8(x1,x2,x3,x4,x5,x6,x7,x8)    TRACE_START(); PRINT(x1); PRINT(x2); PRINT(x3); PRINT(x4); PRINT(x5); PRINT(x6); PRINT(x7); PRINT(x8);  TRACE_END()

#else

#define PRINT(x)
#define PRINTLN(x)
#define TRACE_START()
#define TRACE_END()

#define TRACE_1(...)
#define TRACE_2(...)
#define TRACE_3(...)
#define TRACE_4(...)
#define TRACE_5(...)
#define TRACE_6(...)
#define TRACE_7(...)
#define TRACE_8(...)

#endif

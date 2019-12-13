#include "layer.h"
#include "looper.h"
#include "midi.h"

#include <Arduino.h>

namespace midiate
{

namespace
{

#define ASSERT(x, expected) static_assert(static_cast<int>(x) == (expected), "Expected midiate::" #x " to be equal to " #expected);

#define STYLE(...)                                                      \
    [](int index) -> Degree                                             \
    {                                                                   \
        constexpr Degree __notes[] = { __VA_ARGS__ };                   \
        constexpr int __count = sizeof(__notes) / sizeof(__notes[0]);   \
                                                                        \
        static_assert(Layer::Period % __count == 0,                     \
            "Invalid # of style notes");                                \
                                                                        \
        return __notes[index % __count];                                \
    }

using style_t = Degree(*)(int);

const style_t __styles[] =
    {
        STYLE(1, 3, 5),
        STYLE(5, 3, 1),
        STYLE(1, 3, 5, 3),
        STYLE(5, 3, 1, 3),
        STYLE(1, 3, 5, 3, 1),
        STYLE(5, 3, 1, 3, 5),
        STYLE(1, 5, 3),
        STYLE(3, 1, 5),
        STYLE(1, 5, 3, 5),
        STYLE(1, 3, 1, 5),
    };

ASSERT(Style::Up,           0);
ASSERT(Style::Down,         1);
ASSERT(Style::UpDown,       2);
ASSERT(Style::DownUp,       3);
ASSERT(Style::UpAndDown,    4);
ASSERT(Style::DownAndUp,    5);
ASSERT(Style::Converge,     6);
ASSERT(Style::Diverge,      7);
ASSERT(Style::PinkyUp,      8);
ASSERT(Style::ThumbUp,      9);

static_assert(sizeof(__styles) / sizeof(__styles[0]) == 10, "Expected 10 styles to be declared");

inline float fractional(float x)
{
    return x - (long)x;
}

inline unsigned integer(float x)
{
    return x;
}

#define RHYTHM(...)                                                             \
    [](const Layer & layer, const Time & now) -> int                            \
    {                                                                           \
        constexpr float __portions[] = { __VA_ARGS__ };                         \
        constexpr int __count = sizeof(__portions) / sizeof(__portions[0]);     \
                                                                                \
        static_assert(Layer::Period % __count == 0,                             \
            "Invalid # of rhythm notes");                                       \
                                                                                \
        unsigned length = 1; /* # of bars in the rhythm */                      \
        for (auto portion : __portions)                                         \
        {                                                                       \
            length = max(length, integer(portion) + 1);                         \
        }                                                                       \
                                                                                \
        const auto difference = now - layer.start;                              \
                                                                                \
        for (unsigned i = 0; i < __count; ++i)                                  \
        {                                                                       \
            /* how many subdivisions should pass */                             \
            const unsigned subdivisions =                                       \
                fractional(__portions[i]) * Time::Subdivisions;                 \
                                                                                \
            if (difference.subdivisions != subdivisions)                        \
            {                                                                   \
                continue;                                                       \
            }                                                                   \
                                                                                \
            /* the bar index in which the note should be played */              \
            const unsigned bar = integer(__portions[i]);                        \
                                                                                \
            if (difference.bars % length != bar)                                \
            {                                                                   \
                continue;                                                       \
            }                                                                   \
                                                                                \
            return i + ((layer.counter / length) * __count);                    \
        }                                                                       \
                                                                                \
        return -1;                                                              \
    }

using rhythm_t = int(*)(const Layer &, const Time &);

#define THIRD(i)    (i / 3.f)
#define QUARTER(i)  (i / 4.f)

const rhythm_t __rhythms[] =
    {
        RHYTHM(QUARTER(0), QUARTER(1), QUARTER(2), QUARTER(3)), // 1/16 1/16 1/16 1/16
        RHYTHM(QUARTER(0), QUARTER(1), QUARTER(2)),             // 1/16 1/16 1/8
        RHYTHM(QUARTER(0),             QUARTER(2), QUARTER(3)), // 1/8       1/16 1/16
        RHYTHM(QUARTER(0), QUARTER(1),             QUARTER(3)), // 1/16 1/8       1/16
        RHYTHM(            QUARTER(1), QUARTER(2), QUARTER(3)), //      1/16 1/16 1/16
        RHYTHM(THIRD(0),      THIRD(1),      THIRD(2)),         // 1/8th note triplet
        RHYTHM(THIRD(0),                     THIRD(2)),         // 1/8th note swung triplet
        RHYTHM(THIRD(0), THIRD(3),           THIRD(5)),         // 2-bar 1/8th note swing
    };

ASSERT(Rhythm::A,   0);
ASSERT(Rhythm::B,   1);
ASSERT(Rhythm::C,   2);
ASSERT(Rhythm::D,   3);
ASSERT(Rhythm::E,   4);
ASSERT(Rhythm::F,   5);
ASSERT(Rhythm::G,   6);
ASSERT(Rhythm::H,   7);

static_assert(sizeof(__rhythms) / sizeof(__rhythms[0]) == 8, "Expected 8 rhythms to be declared");

} //

Layer::Layer(char tag, Degree chord, const Time & now) :
    tag(tag),
    chord(chord),
    start(now)
{}

#define ORDERED(a,b,c) (((a) <= (b)) && ((b) <= (c)))

bool Layer::play(const Time & now, Style style, Rhythm rhythm, /* out */ Pitch & pitch)
{
    const auto index = __rhythms[(int)rhythm](*this, now);

    if (index == -1)
    {
        return false; // not supposed to be played right now
    }

    if (state == State::Playback                &&
        !ORDERED(record.start, now, record.end) &&
        !ORDERED(record.end, record.start, now) &&
        !ORDERED(now, record.end, record.start))
    {
        return false; // not supposed to be played right now
    }

    /* out */ pitch = Pitch(chord, __styles[(int)style](index));
    return true;
}

} // midiate

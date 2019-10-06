#include "layer.h"
#include "looper.h"
#include "midi.h"

namespace midiate
{

namespace
{

#define SEQUENCE(...) { __VA_ARGS__ , -1 } // '-1' indicates the end of the sequence

const char __styles[][Layer::Pitches + 1] = // '+1' for the end indicator
    {
        SEQUENCE(1, 3, 5),          // up
        SEQUENCE(5, 3, 1),          // down
        SEQUENCE(1, 3, 5, 3),       // up down
        SEQUENCE(5, 3, 1, 3),       // down up
        SEQUENCE(1, 3, 5, 3, 1),    // up & down
        SEQUENCE(5, 3, 1, 3, 5),    // down & up
        SEQUENCE(1, 5, 3),          // converge
        SEQUENCE(3, 1, 5),          // diverge
        SEQUENCE(1, 5, 3, 5),       // pinky up
        SEQUENCE(1, 3, 1, 5),       // thumb up
    };

#define ASSERT(x, expected) static_assert(static_cast<int>(x) == (expected), "Expected midiate::" #x " to be equal to " #expected);

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

#define THIRD(i)    (i / 3.f)
#define QUARTER(i)  (i / 4.f)

const float __rhythms[][Layer::Moments + 1] = // '+1' for the end indicator
    {
        SEQUENCE(QUARTER(0), QUARTER(1), QUARTER(2), QUARTER(3)),   // 1/16 1/16 1/16 1/16
        SEQUENCE(QUARTER(0), QUARTER(1), QUARTER(2)),               // 1/16 1/16 1/8
        SEQUENCE(QUARTER(0),             QUARTER(2), QUARTER(3)),   // 1/8       1/16 1/16
        SEQUENCE(QUARTER(0), QUARTER(1),             QUARTER(3)),   // 1/16 1/8       1/16
        SEQUENCE(            QUARTER(1), QUARTER(2), QUARTER(3)),   //      1/16 1/16 1/16
        SEQUENCE(THIRD(0),      THIRD(1),      THIRD(2)),           // 1/8th note triplet
        SEQUENCE(THIRD(0),                     THIRD(2)),           // 1/8th note swung triplet
        SEQUENCE(THIRD(0), THIRD(3),           THIRD(5)),           // 2-bar 1/8th note swing
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

Layer::Layer(const Triad & triad, Style style, Rhythm rhythm, char start, char tag) :
    tag(tag)
{
    char const * const degrees = __styles[static_cast<int>(style)];

    for (auto i = 0; degrees[i] != -1; ++i)
    {
        _pitches[i] = triad.degree(degrees[i]);
    }

    float const * const portions = __rhythms[static_cast<int>(rhythm)];

    for (auto i = 0; portions[i] != -1; ++i)
    {
        _moments[i].subdivision = (start + static_cast<int>(portions[i] * Beat::Subdivisions)) % Beat::Subdivisions;
    }
}

void Layer::play(const Beat & now)
{
    auto & next = *_moments;

    if (next.subdivision != now.subdivision)
    {
        return;
    }

    if (now.bar != -1)
    {
        const auto bit = 1L << now.bar;

        static_assert(sizeof(bit) == sizeof(next.bars), "Sizes must match");

        if (_state == State::Record)
        {
            next.bars |= bit;
        }
        else if (_state == State::Playback)
        {
            if ((next.bars & bit) == 0)
            {
                return; // the next moment should not be played in this bar
            }
        }
        
        // nothing to do when wandering
    }

    midi::play(*_pitches);

    ++_moments;
    ++_pitches;
}

void Layer::record()
{
    _state = State::Record;
}

void Layer::playback()
{
    _state = State::Playback;
    
    _moments.revoke(
        [](const Moment & moment)
            {
                return moment.bars == 0; // ignore unrecorded moments
            }
    );
}

} // midiate

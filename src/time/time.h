#pragma once

namespace midier
{

struct Time
{
    Time() = default;
    Time(char bar, char subdivision);

    // the following types can be 'char' as long as Time::Bars and Time::Subdivisions are less than 127
    char bar = -1;
    char subdivision = 0;

    void operator++();

    // the # of bars in the logical loop
    // this should be a large number to make it seem continous and infinite
    constexpr static auto Bars = 48;

    // bars are in 1/4 time signature
    // we support the following note values:
    //   1) 1/16 (4 notes in a bar)
    //   2) 1/8 (2 notes in a bar)
    //   3) 1/8 triplet (3 notes in a bar)
    // therefore, we use a multiply of 12 for the number of subdivisions in a bar,
    // as it gives us the needed rhythmic precision
    // we use 96 in order to shorten the delay after every subdivision played
    //
    constexpr static auto Subdivisions = 96;

    struct Difference
    {
        char bars;
        char subdivisions;

        unsigned total() const; // total number of subdivisions
    };

    using Duration = Difference;

    static Time now;
};

bool operator==(const Time & lhs, const Time & rhs);
bool operator!=(const Time & lhs, const Time & rhs);
Time::Difference operator-(const Time & to, const Time & from);

} // midier

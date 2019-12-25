#include "style.h"

namespace midiate
{
namespace style
{

namespace
{

#define STYLER(...)                                                         \
    [](unsigned index) -> Degree                                            \
    {                                                                       \
        constexpr Degree __notes[] = { __VA_ARGS__ };                       \
        constexpr unsigned __count = sizeof(__notes) / sizeof(__notes[0]);  \
                                                                            \
        static_assert(60 % __count == 0, "Invalid # of style notes");       \
                                                                            \
        return __notes[index % __count];                                    \
    }

using styler_t = Degree(*)(unsigned);

const styler_t __stylers[] =
    {
        STYLER(1, 3, 5),
        STYLER(5, 3, 1),
        STYLER(1, 3, 5, 3),
        STYLER(5, 3, 1, 3),
        STYLER(1, 3, 5, 3, 1),
        STYLER(5, 3, 1, 3, 5),
        STYLER(1, 5, 3),
        STYLER(3, 1, 5),
        STYLER(1, 5, 3, 5),
        STYLER(1, 3, 1, 5),
        STYLER(1, 3, 1, 5, 1, 3),
        STYLER(5, 3, 5, 1, 5, 3),
    };

static_assert(sizeof(__stylers) / sizeof(__stylers[0]) == (unsigned)Style::Count, "Unexpected number of styles declared");

} //

Degree degree(Style style, unsigned index)
{
    return __stylers[(unsigned)style](index);
}

} // style
} // midiate

#pragma once

namespace midier
{

enum class Rate : char
{
    // values are the number of units that exist in a whole bar
    // bars are in 1/4 time signature

    _1_4        = 1,
    _1_8        = 2,
    _1_16       = 4,
    triplet     = 3,
    sextuplet   = 6,
};

#define ALIASED(__x,__y)    \
    __x,                    \
    __y = __x

enum class Rhythm : char
{
    ALIASED(_0,  Quarter),          // 1/4                        [*           ]
    ALIASED(_1,  Eighth),           // 1/8       1/8              [*     *     ]
    ALIASED(_2,  Sixteenth),        // 1/16 1/16 1/16 1/16        [*  *  *  *  ]
    ALIASED(_3,  One_E_And),        // 1/16 1/16 1/8              [*  *  *     ]
    ALIASED(_4,  One_And_A),        // 1/8       1/16 1/16        [*     *  *  ]
    ALIASED(_5,  One_E_A),          // 1/16 1/8       1/16        [*  *     *  ]
    ALIASED(_6,  E_And_A),          //      1/16 1/16 1/16        [   *  *  *  ]
    ALIASED(_7,  Triplet),          // 1/8th note triplet         [*   *   *   ]
    ALIASED(_8,  Swung_Triplet),    // 1/8th note swung triplet   [*       *   ]
    ALIASED(_9,  Two_Bar_Swing),    // 2-bar 1/8th note swing     [*      (*)  ]
    ALIASED(_10, Sextuplet),        // 1/16th note sextuplet      [* * * * * * ]
    ALIASED(_11, Swung_Sextuplet),  // 1/16th note swung tuplet   [*   * *   * ]

    // last element to indicate the # of actual elements
    Count,
};

#undef ALIASED

struct Layer;

namespace rhythm
{

using Description = char[15]; // '[' + 12 + ']' + '\0'

void description(Rhythm rhythm, /* out */ Description & desc);
bool played(Rhythm rhythm, const Layer & layer, /* out */ unsigned & index);
Rate rate(Rhythm rhythm);

} // rhythm

} // midier

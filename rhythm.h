#pragma once

namespace midiate
{

enum class Rhythm : char
{
    A,  // 1/16 1/16 1/16 1/16          [*  *  *  *  ]
    B,  // 1/16 1/16 1/8                [*  *  *     ]
    C,  // 1/8       1/16 1/16          [*     *  *  ]
    D,  // 1/16 1/8       1/16          [*  *     *  ]
    E,  //      1/16 1/16 1/16          [   *  *  *  ]
    F,  // 1/8th note triplet           [*   *   *   ]
    G,  // 1/8th note swuang triplet    [*       *   ]
    H,  // 2-bar 1/8th note swing       [*           *       *   ]
};

} // midiate

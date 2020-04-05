#pragma once

#include "../degree/degree.h"
#include "../interval/interval.h"
#include "../mode/mode.h"
#include "../quality/quality.h"

namespace midiate
{
namespace scale
{

Interval interval(Mode mode, Degree degree);
Quality  quality (Mode mode, Degree degree);

} // scale
} // midiate

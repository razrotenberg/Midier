#pragma once

#include "../degree/degree.h"
#include "../interval/interval.h"
#include "../mode/mode.h"
#include "../quality/quality.h"

namespace midier
{
namespace scale
{

Interval interval(Mode mode, Degree degree);
Quality  quality (Mode mode, Degree degree);

} // scale
} // midier

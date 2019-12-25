#pragma once

#include "degree.h"
#include "interval.h"
#include "mode.h"
#include "quality.h"

namespace midiate
{
namespace scale
{

Interval interval(Mode mode, Degree degree);
Quality  quality (Mode mode, Degree degree);

} // scale
} // midiate

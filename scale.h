#pragma once

#include "interval.h"
#include "mode.h"
#include "quality.h"

namespace midiate
{
namespace scale
{

Interval interval(Mode mode, char degree);
Quality  quality (Mode mode, char degree);

} // scale
} // midiate

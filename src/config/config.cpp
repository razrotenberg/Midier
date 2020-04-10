#include "config.h"

namespace midier
{

Config & Config::global()
{
    static Config __config;
    return __config;
}

} // midier

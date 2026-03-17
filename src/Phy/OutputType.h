#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#ifndef OutputType_h
#define OutputType_h

namespace LibLanc
{
namespace Phy
{

enum class OutputType
{
#if defined(OUTPUT_OPEN_DRAIN)
    OpenCollector,
#endif
    PushPull
};

}  // namespace Phy
}  // namespace LibLanc

#endif  // OutputType_h
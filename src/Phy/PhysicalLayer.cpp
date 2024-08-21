#include "PhysicalLayer.h"

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

namespace LibLanc
{
namespace Phy
{

PhysicalLayer::PhysicalLayer(bool isInverted)
{
    if (isInverted)
    {
        _lowValue = HIGH;
        _highValue = LOW;
    }
    else
    {
        _lowValue = LOW;
        _highValue = HIGH;
    }
}

void PhysicalLayer::putOne()
{
    writePinState(_highValue);
}

void PhysicalLayer::putZero()
{
    writePinState(_lowValue);
}

void PhysicalLayer::putIdle()
{
    putZero();
}

bool PhysicalLayer::readState()
{
    return readPinState() == _highValue;
}

}  // namespace Phy
}  // namespace LibLanc
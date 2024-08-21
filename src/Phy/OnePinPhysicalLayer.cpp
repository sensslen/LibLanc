#include "OnePinPhysicalLayer.h"

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

namespace LibLanc
{
namespace Phy
{

OnePinPhysicalLayer::OnePinPhysicalLayer(uint8_t pin, bool isInverted) : PhysicalLayer(isInverted), _pin(pin) {}

void OnePinPhysicalLayer::begin()
{
    pinMode(_pin, INPUT);
}

bool OnePinPhysicalLayer::readPinState()
{
    pinMode(_pin, INPUT);
    return digitalRead(_pin);
}

void OnePinPhysicalLayer::writePinState(const bool state)
{
    pinMode(_pin, OUTPUT);
    digitalWrite(_pin, state);
}

void OnePinPhysicalLayer::putIdle()
{
    pinMode(_pin, INPUT);
}

}  // namespace Phy
}  // namespace LibLanc
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

OnePinPhysicalLayer::OnePinPhysicalLayer(uint8_t pin, bool isInverted) : _pin(pin), _isInverted(isInverted) {}

void OnePinPhysicalLayer::begin()
{
    putIdle();
}

void OnePinPhysicalLayer::putOne()
{
    pinMode(_pin, OUTPUT);
    digitalWrite(_pin, _isInverted ? HIGH : LOW);
}

void OnePinPhysicalLayer::putZero()
{
    pinMode(_pin, OUTPUT);
    digitalWrite(_pin, _isInverted ? LOW : HIGH);
}

void OnePinPhysicalLayer::putIdle()
{
    pinMode(_pin, INPUT);
}

bool OnePinPhysicalLayer::readState()
{
    return digitalRead(_pin) == (_isInverted ? HIGH : LOW);
}

}  // namespace Phy
}  // namespace LibLanc
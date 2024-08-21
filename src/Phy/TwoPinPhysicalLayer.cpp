#include "TwoPinPhysicalLayer.h"

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

namespace LibLanc
{
namespace Phy
{

TwoPinPhysicalLayer::TwoPinPhysicalLayer(uint8_t inputPin, uint8_t outputPin, bool isInverted)
    : PhysicalLayer(isInverted), _inputPin(inputPin), _outputPin(outputPin)
{
}

void TwoPinPhysicalLayer::begin()
{
    pinMode(_inputPin, INPUT);
    pinMode(_outputPin, OUTPUT);
}

bool TwoPinPhysicalLayer::readPinState()
{
    return digitalRead(_inputPin);
}

void TwoPinPhysicalLayer::writePinState(const bool state)
{
    digitalWrite(_outputPin, state);
}

}  // namespace Phy
}  // namespace LibLanc
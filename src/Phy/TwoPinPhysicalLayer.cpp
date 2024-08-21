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

TwoPinPhysicalLayer::TwoPinPhysicalLayer(uint8_t inputPin, uint8_t outputPin, bool invertSend, bool invertReceive)
    : _inputPin(inputPin), _outputPin(outputPin), _invertSend(invertSend), _invertReceive(invertReceive)
{
}

void TwoPinPhysicalLayer::begin()
{
    pinMode(_inputPin, INPUT);
    pinMode(_outputPin, OUTPUT);
    putIdle();
}

void TwoPinPhysicalLayer::putOne()
{
    digitalWrite(_outputPin, _invertSend ? LOW : HIGH);
}

void TwoPinPhysicalLayer::putZero()
{
    digitalWrite(_outputPin, _invertSend ? HIGH : LOW);
}

void TwoPinPhysicalLayer::putIdle()
{
    putOne();
}

bool TwoPinPhysicalLayer::readState()
{
    return digitalRead(_inputPin) == (_invertReceive ? HIGH : LOW);
}

}  // namespace Phy
}  // namespace LibLanc
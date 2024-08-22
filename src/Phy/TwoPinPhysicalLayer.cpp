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

TwoPinPhysicalLayer::TwoPinPhysicalLayer(uint8_t inputPin, uint8_t outputPin, OutputType outputType)
    : _inputPin(inputPin)
    , _outputPin(outputPin)
    , _outputType(outputType == OutputType::OpenCollector ? OUTPUT_OPEN_DRAIN : OUTPUT)
{
}

void TwoPinPhysicalLayer::begin()
{
    pinMode(_inputPin, INPUT);
    pinMode(_outputPin, _outputType);
    putIdle();
}

void TwoPinPhysicalLayer::putOne()
{
    digitalWrite(_outputPin, LOW);
}

void TwoPinPhysicalLayer::putZero()
{
    digitalWrite(_outputPin, HIGH);
}

void TwoPinPhysicalLayer::putIdle()
{
    putZero();
}

bool TwoPinPhysicalLayer::readState()
{
    return digitalRead(_inputPin) == LOW;
}

}  // namespace Phy
}  // namespace LibLanc
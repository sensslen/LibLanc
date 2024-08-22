#include "LancNonBlocking.h"

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

namespace LibLanc
{
namespace App
{

LancNonBlocking::LancNonBlocking(std::unique_ptr<Phy::IPhysicalLayer> physicalLayer)
    : Lanc(std::move(physicalLayer))
    , _transmitBuffer{ 0, 0, 0, 0 }
    , _receiveBuffer{ 0, 0, 0, 0 }
    , _timeStore(0)
    , _currentBit(0)
    , _currentState(&LancNonBlocking::searchStart)
{
    _activeCommand->prepareTransmission(_transmitBuffer);
}

void LancNonBlocking::loop()
{
    (this->*_currentState)();
}

int LancNonBlocking::timePassed()
{
    return (micros() - _timeStore);
}

void LancNonBlocking::searchStart()
{
    if (!_physicalLayer->readState())
    {
        _timeStore = micros();
    }
    if (timePassed() > 3000)
    {
        _currentState = &LancNonBlocking::waitForTransmissionStart;
    }
}

void LancNonBlocking::waitForTransmissionStart()
{
    if (!_physicalLayer->readState())
    {
        _currentBit = 0;
        _timeStore = micros();
        _currentState = &LancNonBlocking::waitToTransmitNextBit;
    }
}

void LancNonBlocking::waitToTransmitNextBit()
{
    uint8_t bitNr = _currentBit % 8;

    // calculate delay from start bit in order to avoid adding up errors
    if (timePassed() >= (bitNr + 1) * LANC_BIT_TIME_US)
    {
        if (transmitNextBit())
        {
            _currentState = &LancNonBlocking::waitToTransmitStopBit;
        }
    }
}

void LancNonBlocking::waitToTransmitStopBit()
{
    // calculate delay from start bit in order to avoid adding up errors
    if (timePassed() >= (8 + 1) * LANC_BIT_TIME_US)
    {
        _physicalLayer->putIdle();
        _currentState = &LancNonBlocking::waitForNextStartBit;
    }
}

void LancNonBlocking::waitForNextStartBit()
{
    // make sure to only start searching for the start condition when the stop
    // condition is at least half way through
    if ((timePassed() > (LANC_COMPLETE_BYTE_TIME - LANC_HALF_BIT_TIME_US)) && !_physicalLayer->readState())
    {
        _timeStore = micros();
        if (_currentBit >= (2 * 8))
        {
            _currentState = &LancNonBlocking::waitToReceiveNextBit;
        }
        else
        {
            _currentState = &LancNonBlocking::waitToTransmitNextBit;
        }
    }
}

void LancNonBlocking::waitToReceiveNextBit()
{
    uint8_t bitNr = _currentBit % 8;

    // calculate delay from start bit in order to avoid adding up errors
    if (timePassed() >= (((bitNr + 1) * LANC_BIT_TIME_US) + LANC_HALF_BIT_TIME_US))
    {
        if (receiveNextBit())
        {
            if (_currentBit >= 8 * 8)
            {
                _activeCommand->bytesReceived(_receiveBuffer);
                switchToNextCommand();
                _activeCommand->prepareTransmission(_transmitBuffer);
                _currentState = &LancNonBlocking::searchStart;
            }
            else
            {
                _currentState = &LancNonBlocking::waitForNextStartBit;
            }
        }
    }
}

bool LancNonBlocking::transmitNextBit()
{
    uint8_t byte = _currentBit / 8;
    uint8_t bit = _currentBit % 8;

    if (_transmitBuffer[byte] & (1 << bit))
    {
        _physicalLayer->putOne();
    }
    else
    {
        _physicalLayer->putZero();
    }

    _currentBit++;
    return bit == 7;
}

bool LancNonBlocking::receiveNextBit()
{
    uint8_t byte = _currentBit / 8;
    uint8_t bit = _currentBit % 8;

    if (_physicalLayer->readState())
    {
        _receiveBuffer[byte - 4] |= 1 << bit;
    }

    _currentBit++;
    return bit == 7;
}

}  // namespace App
}  // namespace LibLanc

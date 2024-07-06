#include "LibLancNonBlocking.h"

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#define LANC_BIT_TIME_US (104)
#define LANC_HALF_BIT_TIME_US ((LANC_BIT_TIME_US) / 2)
#define LANC_COMPLETE_BYTE_TIME (10 * LANC_BIT_TIME_US)

#define LANC_VIDEO_CAMERA_SPECIAL_COMMAND 0b00101000

LancNonBlocking::LancNonBlocking(uint8_t inputPin, uint8_t outputPin)
    : _transmitBuffer{ 0, 0, 0, 0 }, _receiveBuffer{ 0, 0, 0, 0 }
{
    _inputPin = inputPin;
    _outputPin = outputPin;

    _activeCommand = make_shared<Commands::EmptyCommand>();

    currentState = &LancNonBlocking::SearchStart;
    timeStore = 0;
}

void LancNonBlocking::loop()
{
    (this->*currentState)();
}

int LancNonBlocking::timePassed()
{
    return (micros() - timeStore);
}

void LancNonBlocking::searchStart()
{
    _activeCommand->prepareTransmission(_transmitBuffer);

    if (!readState())
    {
        timeStore = micros();
    }
    if (timePassed() > 3000)
    {
        currentState = &LancNonBlocking::waitForTransmissionStart;
    }
}

void LancNonBlocking::waitForTransmissionStart()
{
    if (!readState())
    {
        currentBit = 0;
        timeStore = micros();
        currentState = &LancNonBlocking::waitToTransmitNextBit;
    }
}

void LancNonBlocking::waitToTransmitNextBit()
{
    uint8_t bitNr = currentBit % 8;

    // calculate delay from start bit in order to avoid adding up errors
    if (timePassed() >= (bitNr + 1) * LANC_BIT_TIME_US)
    {
        if (transmitNextBit())
        {
            currentState = &LancNonBlocking::waitToTransmitStopBit;
        }
    }
}

void LancNonBlocking::waitToTransmitStopBit()
{
    // calculate delay from start bit in order to avoid adding up errors
    if (timePassed() >= (8 + 1) * LANC_BIT_TIME_US)
    {
        writeIdle();
        currentState = &LancNonBlocking::waitForNextStartBit;
    }
}

void LancNonBlocking::waitForNextStartBit()
{
    // make sure to only start searching for the start condition when the stop
    // condition is at least half way through
    if ((timePassed() > (LANC_COMPLETE_BYTE_TIME - LANC_HALF_BIT_TIME_US)) && !readState())
    {
        timeStore = micros();
        if (currentBit >= (2 * 8))
        {
            currentState = &LancNonBlocking::waitToReceiveNextBit;
        }
        else
        {
            currentState = &LancNonBlocking::waitToTransmitNextBit;
        }
    }
}

void LancNonBlocking::waitToReceiveNextBit()
{
    uint8_t bitNr = currentBit % 8;

    // calculate delay from start bit in order to avoid adding up errors
    if (timePassed() >= (((bitNr + 1) * LANC_BIT_TIME_US) + LANC_HALF_BIT_TIME_US))
    {
        if (receiveNextBit())
        {
            if (currentBit >= 8 * 8)
            {
                _activeCommand->bytesReceived(_receiveBuffer);
                switchToNextCommand();
                currentState = &LancNonBlocking::searchStart;
            }
            else
            {
                currentState = &LancNonBlocking::waitForNextStartBit;
            }
        }
    }
}

bool LancNonBlocking::transmitNextBit()
{
    uint8_t byte = currentBit / 8;
    uint8_t bit = currentBit % 8;

    if (_transmitBuffer[byte] & (1 << bit))
    {
        putOne();
    }
    else
    {
        putZero();
    }

    currentBit++;
    return bit == 7;
}

bool LancNonBlocking::receiveNextBit()
{
    uint8_t byte = currentBit / 8;
    uint8_t bit = currentBit % 8;

    if (readState())
    {
        _receiveBuffer[byte - 4] |= 1 << bit;
    }

    currentBit++;
    return bit == 7;
}

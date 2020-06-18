#include "LibLanc.h"

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#define LANC_BIT_TIME_US (104)
#define LANC_STARTBIT_TIME_US (LANC_BIT_TIME_US)
#define LANC_HALF_BIT_TIME_US ((LANC_BIT_TIME_US) / 2)

#define LANC_VIDEO_CAMERA_SPECIAL_COMMAND 0b00101000

Lanc::Lanc(uint8_t inputPin, uint8_t outputPin)
{
    _inputPin = inputPin;
    _outputPin = outputPin;

    memset(_transmitReceiveBuffer, 0xFF, sizeof(_transmitReceiveBuffer));
}

void Lanc::begin()
{
    pinMode(_inputPin, INPUT);
    pinMode(_outputPin, OUTPUT);
}

void Lanc::setTransmitDataVideoCameraSpecialCommand(uint8_t data)
{
    _transmitReceiveBuffer[0] = LANC_VIDEO_CAMERA_SPECIAL_COMMAND;
    _transmitReceiveBuffer[1] = data;
}

bool Lanc::Zoom(int8_t stepSize)
{
    if (stepSize == 0)
    {
        // we actually want to stop zooming which effectively means
        // clearing the command
        ClearCommand();
        return true;
    }
    if ((stepSize > 8) || (stepSize < -8))
    {
        return false;
    }

    setTransmitDataVideoCameraSpecialCommand((stepSize < 0) ? ((-stepSize * 2) + 0x10) : (stepSize * 2));
    return true;
}

void Lanc::Focus(bool far)
{
    setTransmitDataVideoCameraSpecialCommand((far) ? (0x45) : (0x47));
}

void Lanc::AutoFocus()
{
    setTransmitDataVideoCameraSpecialCommand(0x45);
}

void Lanc::ClearCommand()
{
    _transmitReceiveBuffer[0] = 0xFF;
    _transmitReceiveBuffer[1] = 0xFF;
}

void Lanc::loop()
{
    syncTransmission();

    for (uint8_t bytenr = 0; bytenr < 8; bytenr++)
    {
        transmitByte(_transmitReceiveBuffer[0]);
        transmitByte(_transmitReceiveBuffer[1]);
        transmitByte(_transmitReceiveBuffer[2]);
        transmitByte(_transmitReceiveBuffer[3]);
        receiveByte(&_transmitReceiveBuffer[4]);
        receiveByte(&_transmitReceiveBuffer[5]);
        receiveByte(&_transmitReceiveBuffer[6]);
        receiveByte(&_transmitReceiveBuffer[7]);
    }
}

void Lanc::transmitByte(uint8_t byte)
{
    auto startTime = waitStartBit();
    for (uint8_t i = 0; i < 8; i++)
    {
        if (byte & (1 << i))
        {
            transmitOne();
        }
        else
        {
            transmitZero();
        }
        delayUsWithStartTime(startTime, (i + 1) * LANC_BIT_TIME_US + LANC_STARTBIT_TIME_US); // Wait for the bit to be transmitted
    }
    waitNextStart();
}
void Lanc::receiveByte(uint8_t *byte)
{
    *byte = 0;
    auto startTime = waitStartBit();
    for (uint8_t i = 0; i < 8; i++)
    {
        delayUsWithStartTime(startTime, i * LANC_BIT_TIME_US + LANC_HALF_BIT_TIME_US + LANC_STARTBIT_TIME_US);
        if (inputState())
        {
            *byte |= 1 << i;
        }
        delayUsWithStartTime(startTime, (i + 1) * LANC_BIT_TIME_US + LANC_STARTBIT_TIME_US);
    }
    waitNextStart();
}

void Lanc::waitNextStart()
{
    transmitOne();                            // make sure to stop current transmission
    delayMicroseconds(LANC_HALF_BIT_TIME_US); // Make sure to be in the stop bit before waiting for next byte
    while (inputState())
    {
        // Loop as long as the LANC line is +5V during the stop bit or between messages
    }
}

unsigned long Lanc::waitStartBit()
{
    auto startTime = micros();
    delayUsWithStartTime(startTime, LANC_STARTBIT_TIME_US);
    return startTime;
}

void Lanc::transmitOne()
{
    digitalWrite(_outputPin, HIGH);
}

void Lanc::transmitZero()
{
    digitalWrite(_outputPin, LOW);
}

bool Lanc::inputState()
{
    return digitalRead(_inputPin);
}

void Lanc::syncTransmission()
{
    // Sync to next LANC message
    // lanc protocol requires a stop condition that is longer than 5 milliseconds. Since we expect this function
    // to be called periodically and thus have a delay between completion and next start we use a lower value that
    // is still enough to determine between stop conditions during an ongoing transmission and the stop condition
    // between two transmissions.

    // wait for long enough stop condition
    int stopConditionStart = micros();
    do
    {
        if (!inputState())
        {
            stopConditionStart = micros();
        }

        if ((micros() - stopConditionStart < 3000))
        {
            break;
        }
    } while (true);

    // wait for start condition
    while (inputState())
        ;
}

void Lanc::delayUsWithStartTime(unsigned long startTime, unsigned long waitTime)
{
    while ((micros() - startTime) < waitTime)
    {
        // loop until the delay is waited
    }
}
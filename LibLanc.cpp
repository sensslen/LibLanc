#include "LibLanc.h"

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#define LANC_BIT_TIME_US (104)
#define LANC_HALF_BIT_TIME_US ((LANC_BIT_TIME_US) / 2)

#define LANC_VIDEO_CAMERA_SPECIAL_COMMAND 0b00101000

Lanc::Lanc(uint8_t inputPin, uint8_t outputPin)
{
    _inputPin = inputPin;
    _outputPin = outputPin;

    _inputPort = portInputRegister(digitalPinToPort(inputPin));
    _inputPinMask = digitalPinToBitMask(inputPin);
    _outputPort = portOutputRegister(digitalPinToPort(outputPin));
    _outputPinMask = digitalPinToBitMask(outputPin);
}

void Lanc::begin()
{
    pinMode(_inputPin, INPUT);
    pinMode(_outputPin, OUTPUT);
}

void Lanc::tansmitVideoCameraSpecialCommand(uint8_t data)
{
    uint8_t transmitReceive[8] = {LANC_VIDEO_CAMERA_SPECIAL_COMMAND, data};
    lancTransmitReceive(transmitReceive, 4);
}

bool Lanc::Zoom(int8_t stepSize)
{
    if (stepSize == 0)
    {
        return true;
    }
    if ((stepSize > 8) || (stepSize < -8))
    {
        return false;
    }

    tansmitVideoCameraSpecialCommand((stepSize < 0) ? ((-stepSize * 2) + 0x10) : (stepSize * 2));
    return true;
}

void Lanc::Focus(bool far)
{
    tansmitVideoCameraSpecialCommand((far) ? (0x45) : (0x47));
}

void Lanc::AutoFocus()
{
    tansmitVideoCameraSpecialCommand(0x45);
}

void Lanc::lancTransmitReceive(uint8_t transmitReceiveBuffer[8], uint8_t repeats)
{
    // This function is time critical and optimized
    // It takes ~3.2us for the arduino to set a pin state with the digitalWrite command
    // It takes ~80ns for the arduino to set pin state using the direct register method
    // delayMicroseconds(50) ~ 49us, delayMicroseconds(100) ~ 99us

    int i = 0;

    while (pulseIn(_inputPin, HIGH) < 5000)
    {
        // Sync to next LANC message
        // "pulseIn, HIGH" catches any 0V TO +5V TRANSITION and waits until the LANC line goes back to 0V
        // "pulseIn" also returns the pulse duration so we can check if the previous +5V duration was long enough (>5ms) to be the pause before a new 8 byte data packet
    }

    while (repeats)
    {
        for (uint8_t bytenr = 0; bytenr < 8; bytenr++)
        {
            transmitByte(transmitReceiveBuffer[0]);
            transmitByte(transmitReceiveBuffer[1]);
            receiveByte(&transmitReceiveBuffer[2]);
            receiveByte(&transmitReceiveBuffer[3]);
            receiveByte(&transmitReceiveBuffer[4]);
            receiveByte(&transmitReceiveBuffer[5]);
            receiveByte(&transmitReceiveBuffer[6]);
            receiveByte(&transmitReceiveBuffer[7]);
        }
        repeats--;
    }
}

void Lanc::transmitByte(uint8_t byte)
{
    waitStartBit();
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
        delayMicroseconds(LANC_BIT_TIME_US); // Wait for the bit to be transmitted
    }
    waitNextStart();
}
void Lanc::receiveByte(uint8_t *byte)
{
    *byte = 0;
    waitStartBit();
    for (uint8_t i = 0; i < 8; i++)
    {
        delayMicroseconds(LANC_HALF_BIT_TIME_US);
        if (*_inputPort & _inputPinMask)
        {
            *byte |= 1 << i;
        }
        delayMicroseconds(LANC_HALF_BIT_TIME_US);
    }
    waitNextStart();
}

void Lanc::waitNextStart()
{
    transmitOne();                            // make sure to stop current transmission
    delayMicroseconds(LANC_HALF_BIT_TIME_US); // Make sure to be in the stop bit before waiting for next byte
    while (*_inputPort & _inputPinMask)
    {
        // Loop as long as the LANC line is +5V during the stop bit or between messages
    }
}

void Lanc::waitStartBit()
{
    delayMicroseconds(LANC_BIT_TIME_US);
}

void Lanc::transmitOne()
{
    *_outputPort |= _outputPinMask;
}

void Lanc::transmitZero()
{
    *_outputPort &= ~_outputPinMask;
}
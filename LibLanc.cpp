#include "LibLanc.h"
#include "TimerOne.h"

#define LANC_BIT_TIME_US 104

#define LANC_VIDEO_CAMERA_SPECIAL_COMMAND 0b00101000

Lanc::Lanc(uint8_t inputPin, uint8_t outputPin)
{
    _inputPin = inputPin;
    _outputPin = _outputPin;

    tLancCommand *next = NULL;
    for (auto element : _commandBuffers)
    {
        element.next = next;
        next = &element;
    }
    _lancCommandHeap = next;
    _currentTransmission = null;
    _pendingTransmissions = null;
    _currentBit = 0;
    _waitForStartBit = true;
    memset(_currentTransmission, 0xFF, sizeof(_currentTransmission));
    _pendingTransmission.pending = false;
}

void Lanc::Start(void (*pinHandler)(void), void (*timerHandler)(void))
{
    Timer1.initialize(LANC_BIT_TIME_US);
    Timer1.attachInterrupt(timerHandler);
    attachInterrupt(digitalPinToInterrupt(_inputPin), pinHandler, FALLING);
}

bool transmitCommandData(uint8_t data)
{
    if (_pendingTransmission.pending)
    {
        return false;
    }

    _pendingTransmission.data[0] = LANC_VIDEO_CAMERA_SPECIAL_COMMAND;
    _pendingTransmission.data[1] = data;
    return true;
}

void Lanc::Zoom(int8_t stepSize)
{
    if (stepSize == 0)
    {
        return true;
    }
    if ((stepSize > 8) || (stepSize < -8))
    {
        return false;
    }

    return transmitCommandData((stepSize < 0) ? ((-stepSize * 2) + 0x10) : (stepSize * 2));
}

void Lanc::Focus(bool far)
{
    return transmitCommandData((far) ? (0x45) : (0x47));
}

void Lanc::AutoFocus(bool far)
{
    return transmitCommandData(0x45);
}

void Lanc::PinInterrupt(void)
{
    unsigned long newTime = micros();
    if (_waitForStartBit)
    {
        if (_currentBit == 0)
        {
            // need to wait for first start bit which must occur more than 10 ms after last change
            if ((newTime - _lastPinChange) > 10 * 1000)
            {
                _waitForStartBit = false;
                Timer1.start();

                // copy pending transmission
                if (_currentTransmission.transmitCount == 4)
                {
                    if (_pendingTransmission.pending)
                    {
                        memcpy(_currentTransmission, _pendingTransmission.data, sizeof(_currentTransmission));
                        _pendingTransmission.pending = false;
                        _currentTransmission.transmitCount = 0;
                    }
                    else
                    {
                        // stay idle if there is nothing to transmit
                        memset(_currentTransmission, 0xFF, sizeof(_currentTransmission));
                    }
                }
            }
        }
        else if ((_currentBit & 0b111) == 0)
        {
            // need to wait for first start bit which must occur more than 1 ms after last change
            if ((newTime - _lastPinChange) > 1000)
            {
                _waitForStartBit = false;
                Timer1.start();
            }
        }
    }

    _lastPinChange = newTime;
}

void Lanc::TimerInterrupt(void)
{
    if (_waitForStartBit)
    {
        // set idle
        digitalWrite(_outputPin, HIGH);
        Timer1.stop();
        if (_currentBit == 0 && _currentTransmission.transmitCount < 4)
        {
            _currentTransmission.transmitCount++;
        }
    }
    else
    {
        auto currentByte = _currentBit >> 3;
        auto currentBit = _currentBit & 0b111;

        digitalWrite(_outputPin, ((_currentTransmission[currentByte] & (1 << currentBit)) > 0) ? HIGH : LOW);
        if (currentBit == 0b111)
        {
            _waitForStartBit = true;
        }
        _currentBit++; // overflow is desired here, as we transmit a total of 256 bits
    }
}

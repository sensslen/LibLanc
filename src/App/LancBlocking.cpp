#include "LancBlocking.h"

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

namespace LibLanc
{
namespace App
{

LancBlocking::LancBlocking(std::unique_ptr<Phy::PhysicalLayer> physicalLayer) : Lanc(std::move(physicalLayer)) {}

void LancBlocking::loop()
{
    uint8_t transmitBuffer[4];
    uint8_t receiveBuffer[4];
    _activeCommand->prepareTransmission(transmitBuffer);

    auto startTime = syncTransmission();

    transmitByte(transmitBuffer[0], startTime);
    startTime = waitNextStart();
    transmitByte(transmitBuffer[1], startTime);
    startTime = waitNextStart();
    transmitByte(transmitBuffer[2], startTime);
    startTime = waitNextStart();
    transmitByte(transmitBuffer[3], startTime);
    startTime = waitNextStart();
    receiveBuffer[0] = receiveByte(startTime);
    startTime = waitNextStart();
    receiveBuffer[1] = receiveByte(startTime);
    startTime = waitNextStart();
    receiveBuffer[2] = receiveByte(startTime);
    startTime = waitNextStart();
    receiveBuffer[3] = receiveByte(startTime);

    _activeCommand->bytesReceived(receiveBuffer);
    switchToNextCommand();
}

void LancBlocking::transmitByte(uint8_t byte, unsigned long startTime)
{
    waitStartBitComplete(startTime);
    for (uint8_t i = 0; i < 8; i++)
    {
        if (byte & (1 << i))
        {
            _physicalLayer->putOne();
        }
        else
        {
            _physicalLayer->putZero();
        }
        delayUsWithStartTime(
            startTime, (i + 1) * LANC_BIT_TIME_US + LANC_STARTBIT_TIME_US);  // Wait for the bit to be transmitted
    }
}
uint8_t LancBlocking::receiveByte(unsigned long startTime)
{
    uint8_t byte = 0;
    waitStartBitComplete(startTime);
    for (uint8_t i = 0; i < 8; i++)
    {
        delayUsWithStartTime(startTime, i * LANC_BIT_TIME_US + LANC_HALF_BIT_TIME_US + LANC_STARTBIT_TIME_US);
        if (_physicalLayer->readState())
        {
            byte |= 1 << i;
        }
        delayUsWithStartTime(startTime, (i + 1) * LANC_BIT_TIME_US + LANC_STARTBIT_TIME_US);
    }
    return byte;
}

unsigned long LancBlocking::waitNextStart()
{
    _physicalLayer->putIdle();                 // make sure to stop current transmission
    delayMicroseconds(LANC_HALF_BIT_TIME_US);  // Make sure to be in the stop bit before waiting for next byte

    return waitForStartBit();
}

void LancBlocking::waitStartBitComplete(unsigned long startTime)
{
    delayUsWithStartTime(startTime, LANC_STARTBIT_TIME_US);
}

unsigned long LancBlocking::syncTransmission()
{
    // Sync to next LANC message
    // lanc protocol requires a stop condition that is longer than 5 milliseconds. Since we expect this function
    // to be called periodically and thus have a delay between completion and next start we use a lower value that
    // is still enough to determine between stop conditions during an ongoing transmission and the stop condition
    // between two transmissions.

    // wait for long enough stop condition
    int stopConditionStart = micros();
    while ((micros() - stopConditionStart) < 3000)
    {
        if (!_physicalLayer->readState())
        {
            stopConditionStart = micros();
        }
    }

    return waitForStartBit();
}

void LancBlocking::delayUsWithStartTime(unsigned long startTime, unsigned long waitTime)
{
    while ((micros() - startTime) < waitTime)
    {
        // loop until the delay is waited
    }
}

unsigned long LancBlocking::waitForStartBit()
{
    unsigned long startTime = micros();
    while (_physicalLayer->readState())
    {
        startTime = micros();
    }
    return startTime;
}

}  // namespace App
}  // namespace LibLanc

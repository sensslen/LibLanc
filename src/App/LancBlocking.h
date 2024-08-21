#ifndef LibLanc_Blocking_h
#define LibLanc_Blocking_h

#include <stdint.h>

#include <memory>

#include "Lanc.h"

namespace LibLanc
{
namespace App
{

class LancBlocking : public Lanc
{
  public:
    /**
     * Setup the lanc instance by assigning Pins to be used.
     * This function also starts the Timer.
     * @param inputPin  The pin tu use to read LANC signals
     * @param outputPin The Pin to use to send LANC signals
     */
    LancBlocking(std::unique_ptr<Phy::PhysicalLayer> physicalLayer);

    /**
     * This function must be called as quickly as possible in order to not loose
     * synchrionization. If the time between two calls is too long we might
     * loose synchronization and thus transmit invalid values. Since the
     * lanc protocol needs approximately 4 repetitions this will most likely cause
     * the commands to not pass through to the camera.
     *
     * @note This function blocks until the command has finished. This is done to ensure
     *       proper timing. This might be improved by using timers and such.
     */
    void loop() override;

  private:
    /**
     * Optimized fucntion that does the actual transmission/reception.
     * @param transmitReceiveBuffer Buffer that contains the transmission data and
     *                              will contain the received data after the data
     *                              exchange completes
     * @param repeats               Number of transmissions to execute before returning.
     *                              lanc needs 4 Transmissions in order to safely transmit.
     */
    void lancTransmitReceive(uint8_t transmitReceiveBuffer[8], uint8_t repeats);

    unsigned long syncTransmission();
    void transmitByte(uint8_t byte, unsigned long startTime);
    uint8_t receiveByte(unsigned long startTime);
    unsigned long waitNextStart();
    unsigned long waitForStartBit();
    void waitStartBitComplete(unsigned long startTime);
    void delayUsWithStartTime(unsigned long startTime, unsigned long waitTime);

    const uint8_t LANC_STARTBIT_TIME_US = LANC_BIT_TIME_US;
};

}  // namespace App
}  // namespace LibLanc

#endif  // LibLanc_Blocking_h

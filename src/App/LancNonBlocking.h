#include <stdint.h>

#include <memory>

#include "Lanc.h"

#ifndef LibLanc_NonBlocking_h
#define LibLanc_NonBlocking_h

namespace LibLanc
{
namespace App
{

class LancNonBlocking : public Lanc
{
  public:
    /**
     * Setup the lanc instance by assigning Pins to be used.
     * This function also starts the Timer.
     * @param inputPin  The pin tu use to read LANC signals
     * @param outputPin The Pin to use to send LANC signals
     */
    LancNonBlocking(std::unique_ptr<Phy::IPhysicalLayer> physicalLayer);

    /**
     * This function must be called as quickly as possible in order to not loose
     * synchrionization. If the time between two calls is too long we might
     * loose synchronization and thus transmit invalid values. Since the
     * lanc protocol needs approximately 4 repetitions this will most likely cause
     * the commands to not pass through to the camera.
     *
     * @note This function checks whether ther is something to do or not.
     *       If there is nothing to do, it retuns immediately
     */
    void loop() override;

  private:
    uint8_t _transmitBuffer[4];
    uint8_t _receiveBuffer[4];
    int _timeStore;
    uint8_t _currentBit;

    // State machine
    /**
     * This variable represnets the current state of the
     * transmission state machine.
     */
    void (LancNonBlocking::*_currentState)();

    /**
     * State representing the fact that we are currently searching for
     * a Lanc start condition.
     */
    void searchStart();
    void waitForTransmissionStart();
    void waitToTransmitNextBit();
    void waitToTransmitStopBit();
    void waitForNextStartBit();
    void waitToReceiveNextBit();

    /**
     * Transmits the next bit.
     * @retval  false The current byte is not yet complete
     * @retval  true  The current bit is the last bit of the current byte
     */
    bool transmitNextBit();
    bool receiveNextBit();
    int timePassed();

    const uint16_t LANC_COMPLETE_BYTE_TIME = (10 * LANC_BIT_TIME_US);
};

}  // namespace App
}  // namespace LibLanc

#endif  // LibLanc_NonBlocking_h

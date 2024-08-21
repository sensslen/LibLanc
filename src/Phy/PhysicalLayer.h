#ifndef PhysicalLayer_h
#define PhysicalLayer_h

#include <stdint.h>

#include "ILancCommand.h"

namespace LibLanc
{
namespace Phy
{

class PhysicalLayer
{
  public:
    /**
     * @brief Put a one value onto the bus
     *
     */
    void putOne();

    /**
     * @brief Put a zero value onto the bus
     *
     */
    void putZero();

    /**
     * @brief Make sure the bus is idle
     *
     */
    void putIdle();

    /**
     * @brief Read the current state of the bus
     *
     * @return true The bus currently reads as one
     * @return false The bus currently reads as zero
     */
    bool readState();

    /**
     * Setup the pins
     */
    virtual void begin() = 0;

  protected:
    PhysicalLayer(bool isInverted);

    virtual bool readPinState() = 0;
    virtual void writePinState(const bool state) = 0;

  private:
    uint8_t _lowValue;
    uint8_t _highValue;
};

}  // namespace Phy
}  // namespace LibLanc

#endif  // PhysicalLayer_h

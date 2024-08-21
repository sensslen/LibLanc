#include <stdint.h>

#include "ILancCommand.h"

#ifndef IPhysicalLayer_h
#define IPhysicalLayer_h

namespace LibLanc
{
namespace Phy
{

class IPhysicalLayer
{
  public:
    /**
     * @brief Put a one value onto the bus
     *
     */
    virtual void putOne() = 0;

    /**
     * @brief Put a zero value onto the bus
     *
     */
    virtual void putZero() = 0;

    /**
     * @brief Make sure the bus is idle
     *
     */
    virtual void putIdle() = 0;

    /**
     * @brief Read the current state of the bus
     *
     * @return true The bus currently reads as one
     * @return false The bus currently reads as zero
     */
    virtual bool readState() = 0;

    /**
     * Setup the pins
     */
    virtual void begin() = 0;
};

}  // namespace Phy
}  // namespace LibLanc

#endif  // IPhysicalLayer_h

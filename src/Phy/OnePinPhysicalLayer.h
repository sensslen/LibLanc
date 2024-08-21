#include "PhysicalLayer.h"

#ifndef OnePinPhysicalLayer_h
#define OnePinPhysicalLayer_h

namespace LibLanc
{
namespace Phy
{

class OnePinPhysicalLayer : public PhysicalLayer
{
  public:
    OnePinPhysicalLayer(uint8_t pin, bool isInverted);
    virtual void begin() override;

  protected:
    virtual bool readPinState() override;
    virtual void writePinState(const bool state) override;

  private:
    uint8_t _pin;
};
}  // namespace Phy
}  // namespace LibLanc

#endif
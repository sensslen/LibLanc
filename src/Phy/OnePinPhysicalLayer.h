#include "IPhysicalLayer.h"

#ifndef OnePinPhysicalLayer_h
#define OnePinPhysicalLayer_h

namespace LibLanc
{
namespace Phy
{

class OnePinPhysicalLayer : public IPhysicalLayer
{
  public:
    OnePinPhysicalLayer(uint8_t pin, bool isInverted);

    virtual void putOne() override;
    virtual void putZero() override;
    virtual void putIdle() override;
    virtual bool readState() override;
    virtual void begin() override;

  private:
    uint8_t _pin;
    bool _isInverted;
};
}  // namespace Phy
}  // namespace LibLanc

#endif
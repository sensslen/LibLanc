#include "PhysicalLayer.h"

#ifndef TwoPinPhysicalLayer_h
#define TwoPinPhysicalLayer_h

namespace LibLanc
{
namespace Phy
{

class TwoPinPhysicalLayer : public PhysicalLayer
{
  public:
    TwoPinPhysicalLayer(uint8_t inputPin, uint8_t outputPin, bool isInverted);
    virtual void begin() override;

  protected:
    virtual bool readPinState() override;
    virtual void writePinState(const bool state) override;

  private:
    uint8_t _inputPin;
    uint8_t _outputPin;
};

}  // namespace Phy
}  // namespace LibLanc
#endif
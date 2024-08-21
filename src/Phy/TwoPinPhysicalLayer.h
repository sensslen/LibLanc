#include "IPhysicalLayer.h"

#ifndef TwoPinPhysicalLayer_h
#define TwoPinPhysicalLayer_h

namespace LibLanc
{
namespace Phy
{

class TwoPinPhysicalLayer : public IPhysicalLayer
{
  public:
    TwoPinPhysicalLayer(uint8_t inputPin, uint8_t outputPin, bool invertSend, bool invertReceive);

    virtual void putOne() override;
    virtual void putZero() override;
    virtual void putIdle() override;
    virtual bool readState() override;
    virtual void begin() override;

  private:
    uint8_t _inputPin;
    uint8_t _outputPin;
    bool _invertSend;
    bool _invertReceive;
};

}  // namespace Phy
}  // namespace LibLanc
#endif
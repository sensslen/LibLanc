#include <stdint.h>

#include <memory>

#include "App/Lanc.h"
#include "ILancCommand.h"
#include "Phy/IPhysicalLayer.h"

#ifndef LancBuilder_h
#define LancBuilder_h

namespace LibLanc
{

class LancBuilder
{
  public:
    void UseOnePinPhysicalLayer(uint8_t pin, bool isInverted);
    void UseTwoPinPhysicalLayer(uint8_t inputPin, uint8_t outputPin, bool isSendInverted, bool isReceiveInverted);
    std::unique_ptr<App::Lanc> CreateBlocking();
    std::unique_ptr<App::Lanc> CreateNonBlocking();

  private:
    std::unique_ptr<Phy::IPhysicalLayer> _physicalLayer;
};

}  // namespace LibLanc

#endif  // LancBuilder_h

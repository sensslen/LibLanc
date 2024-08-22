#include <stdint.h>

#include <memory>

#include "App/Lanc.h"
#include "ILancCommand.h"
#include "Phy/IPhysicalLayer.h"
#include "Phy/OutputType.h"

#ifndef LancBuilder_h
#define LancBuilder_h

namespace LibLanc
{

class LancBuilder
{
  public:
    void UseTwoPinPhysicalLayer(uint8_t inputPin, uint8_t outputPin, Phy::OutputType outputType);
    std::unique_ptr<App::Lanc> CreateBlocking();
    std::unique_ptr<App::Lanc> CreateNonBlocking();

  private:
    std::unique_ptr<Phy::IPhysicalLayer> _physicalLayer;
};

}  // namespace LibLanc

#endif  // LancBuilder_h

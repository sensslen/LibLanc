#include <stdint.h>

#include "liblanc_memory.h"

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
    LibLanc::std::unique_ptr<App::Lanc> CreateBlocking();
    LibLanc::std::unique_ptr<App::Lanc> CreateNonBlocking();

  private:
    LibLanc::std::unique_ptr<Phy::IPhysicalLayer> _physicalLayer;
};

}  // namespace LibLanc

#endif  // LancBuilder_h

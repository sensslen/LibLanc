#include "LancBuilder.h"

#include "liblanc_memory.h"
#include "liblanc_utility.h"
#include "App/LancBlocking.h"
#include "App/LancNonBlocking.h"
#include "Phy/TwoPinPhysicalLayer.h"

namespace LibLanc
{

void LancBuilder::UseTwoPinPhysicalLayer(uint8_t inputPin, uint8_t outputPin, Phy::OutputType outputType)
{
    _physicalLayer = liblanc::make_unique<Phy::TwoPinPhysicalLayer>(inputPin, outputPin, outputType);
}

liblanc::unique_ptr<App::Lanc> LancBuilder::CreateBlocking()
{
    if (_physicalLayer == nullptr)
    {
        return liblanc::unique_ptr<App::Lanc>();
    }

    return liblanc::make_unique<App::LancBlocking>(liblanc::move(_physicalLayer));
}

liblanc::unique_ptr<App::Lanc> LancBuilder::CreateNonBlocking()
{
    if (_physicalLayer == nullptr)
    {
        return liblanc::unique_ptr<App::Lanc>();
    }

    return liblanc::make_unique<App::LancNonBlocking>(liblanc::move(_physicalLayer));
}

}  // namespace LibLanc
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
    _physicalLayer = LibLanc::std::make_unique<Phy::TwoPinPhysicalLayer>(inputPin, outputPin, outputType);
}

LibLanc::std::unique_ptr<App::Lanc> LancBuilder::CreateBlocking()
{
    if (_physicalLayer == nullptr)
    {
        return LibLanc::std::unique_ptr<App::Lanc>();
    }

    return LibLanc::std::make_unique<App::LancBlocking>(LibLanc::std::move(_physicalLayer));
}

LibLanc::std::unique_ptr<App::Lanc> LancBuilder::CreateNonBlocking()
{
    if (_physicalLayer == nullptr)
    {
        return LibLanc::std::unique_ptr<App::Lanc>();
    }

    return LibLanc::std::make_unique<App::LancNonBlocking>(LibLanc::std::move(_physicalLayer));
}

}  // namespace LibLanc
#include "LancBuilder.h"

#include "App/LancBlocking.h"
#include "App/LancNonBlocking.h"
#include "Phy/OnePinPhysicalLayer.h"
#include "Phy/TwoPinPhysicalLayer.h"

namespace LibLanc
{

void LancBuilder::UseOnePinPhysicalLayer(uint8_t pin, bool isInverted)
{
    _physicalLayer = std::make_unique<Phy::OnePinPhysicalLayer>(pin, isInverted);
}

void LancBuilder::UseTwoPinPhysicalLayer(uint8_t inputPin, uint8_t outputPin, bool isInverted)
{
    _physicalLayer = std::make_unique<Phy::TwoPinPhysicalLayer>(inputPin, outputPin, isInverted);
}

std::unique_ptr<App::Lanc> LancBuilder::CreateBlocking()
{
    if (_physicalLayer == nullptr)
    {
        return std::unique_ptr<App::Lanc>();
    }

    return std::make_unique<App::LancBlocking>(std::move(_physicalLayer));
}

std::unique_ptr<App::Lanc> LancBuilder::CreateNonBlocking()
{
    if (_physicalLayer == nullptr)
    {
        return std::unique_ptr<App::Lanc>();
    }

    return std::make_unique<App::LancNonBlocking>(std::move(_physicalLayer));
}

}  // namespace LibLanc
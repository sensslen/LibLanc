#include "CommandFactory.h"

namespace LibLanc
{

std::shared_ptr<ILancCommand> CommandFactory::zoom(const uint8_t stepSize)
{
    return std::make_shared<Commands::ZoomCommand>(stepSize);
}

}  // namespace LibLanc

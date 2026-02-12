#include "CommandFactory.h"

#include "Commands/AutoFocusCommand.h"
#include "Commands/EmptyCommand.h"
#include "Commands/FocusCommand.h"
#include "Commands/ZoomCommand.h"

namespace LibLanc
{

std::unique_ptr<ILancCommand> CommandFactory::zoom(const uint8_t stepSize)
{
    return std::make_unique<Commands::ZoomCommand>(stepSize);
}

std::unique_ptr<ILancCommand> CommandFactory::focus(const bool far)
{
    return std::make_unique<Commands::FocusCommand>(far);
}

std::unique_ptr<ILancCommand> CommandFactory::clear()
{
    return std::make_unique<Commands::EmptyCommand>();
}

std::unique_ptr<ILancCommand> CommandFactory::setAutoFocus(const bool enabled)
{
    return std::make_unique<Commands::AutoFocusCommand>(enabled);
}

}  // namespace LibLanc

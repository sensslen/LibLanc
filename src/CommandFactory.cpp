#include "CommandFactory.h"

#include "Commands/AutoFocusCommand.h"
#include "Commands/EmptyCommand.h"
#include "Commands/FocusCommand.h"
#include "Commands/ZoomCommand.h"

namespace LibLanc
{

std::shared_ptr<ILancCommand> CommandFactory::zoom(const uint8_t stepSize)
{
    return std::make_shared<Commands::ZoomCommand>(stepSize);
}

std::shared_ptr<ILancCommand> CommandFactory::focus(const bool far)
{
    return std::make_shared<Commands::FocusCommand>(far);
}

std::shared_ptr<ILancCommand> CommandFactory::clear()
{
    return std::make_shared<Commands::EmptyCommand>();
}

std::shared_ptr<ILancCommand> CommandFactory::setAutoFocus(const bool enabled)
{
    return std::make_shared<Commands::AutoFocusCommand>(enabled);
}

}  // namespace LibLanc

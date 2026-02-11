#include "CommandFactory.h"

#include "liblanc_memory.h"
#include "Commands/AutoFocusCommand.h"
#include "Commands/EmptyCommand.h"
#include "Commands/FocusCommand.h"
#include "Commands/ZoomCommand.h"

namespace LibLanc
{

liblanc::unique_ptr<ILancCommand> CommandFactory::zoom(const uint8_t stepSize)
{
    return liblanc::make_unique<Commands::ZoomCommand>(stepSize);
}

liblanc::unique_ptr<ILancCommand> CommandFactory::focus(const bool far)
{
    return liblanc::make_unique<Commands::FocusCommand>(far);
}

liblanc::unique_ptr<ILancCommand> CommandFactory::clear()
{
    return liblanc::make_unique<Commands::EmptyCommand>();
}

liblanc::unique_ptr<ILancCommand> CommandFactory::setAutoFocus(const bool enabled)
{
    return liblanc::make_unique<Commands::AutoFocusCommand>(enabled);
}

}  // namespace LibLanc

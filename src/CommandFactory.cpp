#include "CommandFactory.h"

#include "liblanc_memory.h"
#include "Commands/AutoFocusCommand.h"
#include "Commands/EmptyCommand.h"
#include "Commands/FocusCommand.h"
#include "Commands/ZoomCommand.h"

namespace LibLanc
{

LibLanc::std::unique_ptr<ILancCommand> CommandFactory::zoom(const uint8_t stepSize)
{
    return LibLanc::std::make_unique<Commands::ZoomCommand>(stepSize);
}

LibLanc::std::unique_ptr<ILancCommand> CommandFactory::focus(const bool far)
{
    return LibLanc::std::make_unique<Commands::FocusCommand>(far);
}

LibLanc::std::unique_ptr<ILancCommand> CommandFactory::clear()
{
    return LibLanc::std::make_unique<Commands::EmptyCommand>();
}

LibLanc::std::unique_ptr<ILancCommand> CommandFactory::setAutoFocus(const bool enabled)
{
    return LibLanc::std::make_unique<Commands::AutoFocusCommand>(enabled);
}

}  // namespace LibLanc

#include "FocusCommand.h"

namespace LibLanc
{
namespace Commands
{

FocusCommand::FocusCommand(bool far)
{
    _commandByte = (far) ? (0x45) : (0x47);
}

uint8_t FocusCommand::getCommand()
{
    return _commandByte;
}

ILancCommand::LancCommandState FocusCommand::getState() const
{
    return ILancCommand::LancCommandState::CanContinue;
}

void FocusCommand::bytesReceived(const uint8_t bytesReceived[4]) {}

}  // namespace Commands
}  // namespace LibLanc

#include "ZoomCommand.h"

namespace LibLanc
{
namespace Commands
{

ZoomCommand::ZoomCommand(int8_t stepSize)
{
    _commandByte = (stepSize > 0) ? (stepSize - 1) * 2 : ((-stepSize - 1) * 2) + 0x10;
}

uint8_t ZoomCommand::getCommand()
{
    return _commandByte;
}

ILancCommand::LancCommandState ZoomCommand::getState() const
{
    return ILancCommand::LancCommandState::CanContinue;
}

void ZoomCommand::bytesReceived(const uint8_t bytesReceived[4]) {}

}  // namespace Commands
}  // namespace LibLanc

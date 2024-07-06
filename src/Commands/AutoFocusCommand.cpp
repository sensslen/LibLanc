#include "AutoFocusCommand.h"

#include <cstring>

namespace LibLanc
{
namespace Commands
{

AutoFocusCommand::AutoFocusCommand(bool enabled)
{
    _commandByte = 0x41;
}

uint8_t AutoFocusCommand::getCommand()
{
    return _commandByte;
}

ILancCommand::LancCommandState AutoFocusCommand::getState() const
{
    // TODO: check if the desired state matches current state. Return ILancCommand::LancCommandState::Continue when not
    // and ILancCommand::LancCommandState::Complete when state is correct
    return ILancCommand::LancCommandState::Complete;
}

void AutoFocusCommand::bytesReceived(const uint8_t bytesReceived[4])
{
    // TODO: check state
}

void AutoFocusCommand::prepareTransmission(uint8_t (&bytes)[4])
{
    std::memset(bytes, 0, sizeof(bytes));
    // TODO: check if the desired state matches current state. Return Zero array when not known or reached, else call
    // VideoCameraSpecialCommand::prepareTransmission
}

}  // namespace Commands
}  // namespace LibLanc

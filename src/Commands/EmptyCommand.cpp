#include "EmptyCommand.h"

#include <cstring>

namespace LibLanc
{
namespace Commands
{

void EmptyCommand::prepareTransmission(uint8_t (&bytes)[4])
{
    std::memset(bytes, 0, sizeof(bytes));
}

ILancCommand::LancCommandState EmptyCommand::getState() const
{
    return ILancCommand::LancCommandState::CanContinue;
}

void EmptyCommand::bytesReceived(const uint8_t bytesReceived[4]) {}

}  // namespace Commands
}  // namespace LibLanc

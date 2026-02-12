#include "Lanc.h"

#include "../liblanc_memory.h"
#include "../liblanc_utility.h"

#include "Commands/EmptyCommand.h"

namespace LibLanc
{
namespace App
{

Lanc::Lanc(LibLanc::std::unique_ptr<Phy::IPhysicalLayer> physicalLayer)
    : _physicalLayer(LibLanc::std::move(physicalLayer)), _activeCommand(LibLanc::std::make_unique<Commands::EmptyCommand>())
{
}

bool Lanc::setCommand(LibLanc::std::unique_ptr<ILancCommand> command)
{
    if (_nextCommand && _nextCommand->getState() == ILancCommand::LancCommandState::Continue)
    {
        return false;
    }

    _nextCommand = LibLanc::std::move(command);
    return true;
}

void Lanc::switchToNextCommand()
{
    switch (_activeCommand->getState())
    {
        case ILancCommand::LancCommandState::Complete:
            if (_nextCommand)
            {
                _activeCommand = LibLanc::std::move(_nextCommand);
            }
            else
            {
                _activeCommand = LibLanc::std::make_unique<Commands::EmptyCommand>();
            }

            break;
        case ILancCommand::LancCommandState::CanContinue:
            if (_nextCommand)
            {
                _activeCommand = LibLanc::std::move(_nextCommand);
            }
            break;
        default:
            break;
    }
}

void Lanc::begin()
{
    _physicalLayer->begin();
}

}  // namespace App
}  // namespace LibLanc

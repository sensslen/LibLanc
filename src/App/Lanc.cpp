#include "Lanc.h"

#include <memory>
#include <utility>

#include "Commands/EmptyCommand.h"

namespace LibLanc
{
namespace App
{

Lanc::Lanc(std::unique_ptr<Phy::IPhysicalLayer> physicalLayer)
    : _physicalLayer(std::move(physicalLayer)), _activeCommand(std::make_unique<Commands::EmptyCommand>())
{
}

bool Lanc::setCommand(std::unique_ptr<ILancCommand> command)
{
    if (_nextCommand && _nextCommand->getState() == ILancCommand::LancCommandState::Continue)
    {
        return false;
    }

    _nextCommand = std::move(command);
    return true;
}

void Lanc::switchToNextCommand()
{
    switch (_activeCommand->getState())
    {
        case ILancCommand::LancCommandState::Complete:
            if (_nextCommand)
            {
                _activeCommand = std::move(_nextCommand);
            }
            else
            {
                _activeCommand = std::make_unique<Commands::EmptyCommand>();
            }

            break;
        case ILancCommand::LancCommandState::CanContinue:
            if (_nextCommand)
            {
                _activeCommand = std::move(_nextCommand);
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

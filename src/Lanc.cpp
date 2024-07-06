#include "Lanc.h"

#include <memory>
#include <utility>

#include "Commands/EmptyCommand.h"

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

namespace LibLanc
{

Lanc::Lanc(uint8_t inputPin, uint8_t outputPin)
{
    _inputPin = inputPin;
    _outputPin = outputPin;

    _activeCommand = std::make_shared<Commands::EmptyCommand>();
}

bool Lanc::setCommand(std::shared_ptr<ILancCommand> command)
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
                _activeCommand = std::make_shared<Commands::EmptyCommand>();
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
    pinMode(_inputPin, INPUT);
    pinMode(_outputPin, OUTPUT);
    putIdle();
}

void Lanc::putOne()
{
    digitalWrite(_outputPin, HIGH);
}

void Lanc::putIdle()
{
    putZero();
}

void Lanc::putZero()
{
    digitalWrite(_outputPin, LOW);
}

bool Lanc::readState()
{
    return digitalRead(_inputPin);
}

}  // namespace LibLanc

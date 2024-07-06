#ifndef Lanc_h
#define Lanc_h

#include <stdint.h>

#include <memory>

#include "ILancCommand.h"

namespace LibLanc
{

class Lanc
{
  public:
    /**
     * Setup the Pins
     */
    void begin();
    /**
     * This function is used in the loop method of the arduino. Please refer to the actual implementation for more
     * details about how to use this method
     */
    virtual void loop() = 0;
    /**
     * Set the next command to be executed by the camera interaction
     * @param command The command to be executed next. This waits for the current command to complete (specify a state
     * other than \ref Commands::ILancCommand::LancCommandState::Continue).
     *
     * @return true: the command was successfully enqueued
     *         false: the command failed to be enqueued
     */
    bool setCommand(std::shared_ptr<ILancCommand> command);

  protected:
    /**
     * Setup the lanc instance by assigning Pins to be used.
     * @param inputPin  The pin tu use to read LANC signals
     * @param outputPin The Pin to use to send LANC signals
     */
    Lanc(uint8_t inputPin, uint8_t outputPin);

    /**
     * @brief switch to the next command to be used depending on the state of the current command and whether there is a
     * next command
     *
     */
    void switchToNextCommand();

    /**
     * @brief Helper method to put a one value onto the bus
     *
     */
    void putOne();

    /**
     * @brief Helper method to put a zero value onto the bus
     *
     */
    void putZero();

    /**
     * @brief Helper method to make sure the bus is idle
     *
     */
    void putIdle();

    /**
     * @brief Hepler method to read the current state of the bus
     *
     * @return true The bus currently reads as one
     * @return false The bus currently reads as zero
     */
    bool readState();

    std::shared_ptr<ILancCommand> _activeCommand;
    std::shared_ptr<ILancCommand> _nextCommand;

    uint8_t _inputPin;
    uint8_t _outputPin;
};

}  // namespace LibLanc

#endif  // Lanc_h

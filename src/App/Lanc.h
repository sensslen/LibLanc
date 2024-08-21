
#include <stdint.h>

#include <memory>

#include "../Phy/IPhysicalLayer.h"
#include "ILancCommand.h"

#ifndef Lanc_h
#define Lanc_h

namespace LibLanc
{
namespace App
{

class Lanc
{
  public:
    /**
     * Setup the Lanc instance
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
    Lanc(std::unique_ptr<Phy::IPhysicalLayer> physicalLayer);

    /**
     * @brief switch to the next command to be used depending on the state of the current command and whether there is a
     * next command
     *
     */
    void switchToNextCommand();

    std::unique_ptr<Phy::IPhysicalLayer> _physicalLayer;

    const uint8_t LANC_BIT_TIME_US = 104;
    const uint8_t LANC_HALF_BIT_TIME_US = (LANC_BIT_TIME_US) / 2;

    std::shared_ptr<ILancCommand> _activeCommand;

  private:
    std::shared_ptr<ILancCommand> _nextCommand;
};

}  // namespace App
}  // namespace LibLanc

#endif  // Lanc_h

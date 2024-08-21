#include "VideoCameraSpecialCommand.h"

#ifndef AutoFocusCommand_h
#define AutoFocusCommand_h

namespace LibLanc
{
namespace Commands
{

/**
 * @brief DO NOT USE. This command's implementation is incomplete, as I don't know of a way to check the current state
 * of the auto focus. Therefore this command is not functional.
 *
 */
class AutoFocusCommand : public VideoCameraSpecialCommand
{
  public:
    /**
     * @brief Command to set the AutoFocus to enabled/disabled
     *
     * @param enabled Whether to enable or disable the auto focus
     */
    AutoFocusCommand(bool enabled);

    /**
     * @copydoc VideoCameraSpecialCommand::getCommand
     */
    uint8_t getCommand() override;

    /**
     * @copydoc ILancCommand::getState
     */
    ILancCommand::LancCommandState getState() const override;

    /**
     * @copydoc ILancCommand::bytesReceived
     */
    void bytesReceived(const uint8_t bytesReceived[4]) override;

    /**
     * @copydoc ILancCommand::prepareTransmission
     */
    void prepareTransmission(uint8_t (&bytes)[4]) override;

  private:
    uint8_t _commandByte;
};

}  // namespace Commands
}  // namespace LibLanc

#endif  // AutoFocusCommand_h

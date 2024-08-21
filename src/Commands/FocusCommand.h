#include "VideoCameraSpecialCommand.h"

#ifndef FocusCommand_h
#define FocusCommand_h

namespace LibLanc
{
namespace Commands
{

class FocusCommand : public VideoCameraSpecialCommand
{
  public:
    /**
     * @brief Change the manual focus
     *
     * @param far Whether to push the Focus farther away (true) or closer (false)
     */
    FocusCommand(bool far);

    /**
     * @copydoc VideoCameraSpecialCommand::GetCommand
     */
    uint8_t getCommand() override;

    /**
     * @copydoc ILancCommand::GetState
     */
    ILancCommand::LancCommandState getState() const override;

    /**
     * @copydoc ILancCommand::BytesReceived
     */
    void bytesReceived(const uint8_t bytesReceived[4]) override;

  private:
    uint8_t _commandByte;
};

}  // namespace Commands
}  // namespace LibLanc

#endif  // FocusCommand_h

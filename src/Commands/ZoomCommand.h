#ifndef ZoomCommand_h
#define ZoomCommand_h

#include "VideoCameraSpecialCommand.h"

namespace LibLanc
{
namespace Commands
{

class ZoomCommand : public VideoCameraSpecialCommand
{
  public:
    /**
     * @brief Create a zoom command. This command instructs the camera to zoom. The zoom level
     * is provided with the stepSize parameter.
     *
     * @param stepSize Zoom in or out. The value must be in the range of [-8..8].
     *                 Negative values zoom out. The value of 0 is silently ignored,
     *                 as it means no zoom and needs no telegram.
     */
    ZoomCommand(int8_t stepSize);

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

#endif  // ZoomCommand_h

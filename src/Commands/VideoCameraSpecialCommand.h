#include "../ILancCommand.h"

#ifndef VideoCameraSpecialCommand_h
#define VideoCameraSpecialCommand_h

namespace LibLanc
{
namespace Commands
{

class VideoCameraSpecialCommand : public ILancCommand
{
  public:
    /**
     * @copydoc ILancCommand::PrepareTransmission
     */
    void prepareTransmission(uint8_t (&bytes)[4]) override;

    /**
     * @brief Get the command byte representing the command. This is the second byte to be transmitted as part of the
     * two byte transmission
     *
     * @return The byte value to be transmitted
     */
    virtual uint8_t getCommand() = 0;
};

}  // namespace Commands
}  // namespace LibLanc

#endif  // VideoCameraSpecialCommand_h

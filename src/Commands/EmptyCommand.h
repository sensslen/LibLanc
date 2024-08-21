#include "../ILancCommand.h"

#ifndef EmptyCommand_h
#define EmptyCommand_h

namespace LibLanc
{
namespace Commands
{

class EmptyCommand : public ILancCommand
{
  public:
    /**
     * @copydoc ILancCommand::GetCommand
     */
    void prepareTransmission(uint8_t (&bytes)[4]) override;

    /**
     * @copydoc ILancCommand::GetState
     */
    ILancCommand::LancCommandState getState() const override;

    /**
     * @copydoc ILancCommand::BytesReceived
     */
    void bytesReceived(const uint8_t bytesReceived[4]) override;
};

}  // namespace Commands
}  // namespace LibLanc

#endif  // EmptyCommand_h

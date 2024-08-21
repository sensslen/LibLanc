#include <stdint.h>

#ifndef ILancCommand_h
#define ILancCommand_h

namespace LibLanc
{

class ILancCommand
{
  public:
    enum LancCommandState
    {
        /// @brief Represents the state that the current command cannot be replaced by the next command (used for
        /// commands that complete - e.g. to switch between auto/manual focus)
        Continue,
        /// @brief Represents the state when the command can continue to send but if there are new commands to be sent,
        /// those can take over (used for commands that continuously send)
        CanContinue,
        /// @brief Represents the state when the command has completed its operation and is due to be retired (used for
        /// commands that complete - e.g. to switch between auto/manual focus)
        Complete
    };

    /**
     * @brief Prepares the bytes to be transmitted.
     *
     * @param bytes Reference to the bytes representing the transmission to the camera.
     * @return The byte to be transmitted
     */
    virtual void prepareTransmission(uint8_t (&bytes)[4]) = 0;

    /**
     * @brief This method is called when the transmission has completed. It's used to inform the command about the bytes
     * that were received
     *
     * @param bytesReceived The data received from the camera
     * [4..8]
     */
    virtual void bytesReceived(const uint8_t bytesReceived[4]) = 0;

    /**
     * @brief Get the state of the current command. This tells the link layer whether to continue with this command or
     * replace it. This value will never be evaluated while a transmission is ongoing, so the implementation must not
     * track the current transmission state. It's the responisbility of the link layer to ensure this.
     *
     * @return LancCommandState The current state of the command
     */
    virtual LancCommandState getState() const = 0;
};

}  // namespace LibLanc

#endif  // ILancCommand_h

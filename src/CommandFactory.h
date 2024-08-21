#include <memory>

#include "ILancCommand.h"

#ifndef CommandFactory_h
#define CommandFactory_h

namespace LibLanc
{

class CommandFactory
{
  public:
    /**
     * @brief Create a zoom command
     *
     * @param stepSize The speed of the zoom. The value must be in the range of [-8..8] where negative values zoom out.
     * Providing a value of 0 is the same as creating a clear command.
     * @return a shared pointer to be used by one of the blocking/non blocking libraries
     */
    static std::shared_ptr<ILancCommand> zoom(const uint8_t stepSize);

    /**
     * @brief Cerate a focus command
     *
     * @param far Whether to shift the focus farther away (true) or closer (false)
     * @return a shared pointer to be used by one of the blocking/non blocking libraries
     */
    static std::shared_ptr<ILancCommand> focus(const bool far);

    /**
     * @brief Create a clear command (basically clears any command that is currently being executed)
     *
     * @return a shared pointer to be used by one of the blocking/non blocking libraries
     */
    static std::shared_ptr<ILancCommand> clear();

    /**
     * @brief Set the Auto Focus object
     *
     * @note Please don't use this method. The auto focus command does not have a proper implementation yet.
     *
     * @param enabled Whether to enable or disable the auto focus
     * @return a shared pointer to be used by one of the blocking/non blocking libraries
     */
    static std::shared_ptr<ILancCommand> setAutoFocus(const bool enabled);
};

}  // namespace LibLanc

#endif
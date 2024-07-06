#include "VideoCameraSpecialCommand.h"

namespace LibLanc
{
namespace Commands
{

#define LANC_VIDEO_CAMERA_SPECIAL_COMMAND 0b00101000

void VideoCameraSpecialCommand::prepareTransmission(uint8_t (&bytes)[4])
{
    bytes[0] = LANC_VIDEO_CAMERA_SPECIAL_COMMAND;
    bytes[1] = getCommand();
    bytes[2] = 0;
    bytes[3] = 0;
}

}  // namespace Commands
}  // namespace LibLanc

#ifndef LibLanc_NonBlocking_h
#define LibLanc_NonBlocking_h

#include <stdint.h>

class LancNonBlocking
{
public:
  /**
   * Setup the lanc instance by assigning Pins to be used.
   * This function also starts the Timer.
   * @param inputPin  The pin tu use to read LANC signals
   * @param outputPin The Pin to use to send LANC signals
   */
  LancNonBlocking(uint8_t inputPin, uint8_t outputPin);
  /**
   * Setup the Pins
   */
  void begin();
  /**
   * This function must be called as quickly as possible in order to not loose
   * synchrionization. If the time between two calls is too long we might
   * loose synchronization and thus transmit invalid values. Since the 
   * lanc protocol needs approximately 4 repetitions this will most likely cause 
   * the commands to not pass through to the camera.
   * 
   * @note This function checks whether ther is something to do or not.
   *       If there is nothing to do, it retuns immediately
   */
  void loop();
  /**
   * Set the zoom speed of the camera. 
   * @param stepSize Zoom in or out. The value must be in the range of [-8..8].
   *                 Negative values zoom out. The value of 0 is silently ignored, 
   *                 as it means no zoom and needs no telegram.
   * @retval  true   Zoom command was successfully transmitted to the camera
   * @retval  false  The passed parameter is out of range
   */
  bool Zoom(int8_t stepSize);
  /**
   * Change the manual focus
   * @param far Whether to pusht the Focus farther away or closer
   */
  void Focus(bool far);
  /**
   * Toggle Autofocus
   * 
   * @note Maybe we could read whether the Autofocus is set by the return value 
   *       and thus change according to the response.
   */
  void AutoFocus();
  /**
   * Set the next lanc transmission to no command
   */
  void ClearCommand();

private:
  /**
   * Setup a new Camera command for tramission.
   * @param data The special Video Camera command that should be transmitted
   *             to the camera
   */
  void setTransmitDataVideoCameraSpecialCommand(uint8_t data);

  void writeOne();
  void writeZero();
  bool readInput();
  int timePassed();
  void swapBuffersIfPending();

  uint8_t transmitBuffers[2][8];
  uint8_t * currentTransmission;
  uint8_t * nextTransmission;
  int timeStore;
  bool bufferSwapPending;
  uint8_t currentBit;

  uint8_t _inputPin;
  uint8_t _outputPin;

  // State machine
  /**
   * This variable represnets the current state of the 
   * transmission state machine.
   */
  void (LancNonBlocking::*currentState)();

  /**
   * State representing the fact that we are currently searching for 
   * a Lanc start condition.
   */
  void SearchStart();
  void WaitForTransmissionStart();
  void WaitToTransmitNextBit();
  void WaitToTransmitStopBit();
  void WaitForNextStartBit();
  void WaitToReceiveNextBit();

  /**
   * Transmits the next bit.
   * @retval  false The current byte is not yet complete
   * @retval  true  The current bit is the last bit of the current byte
   */
  bool TransmitNextBit();
  bool ReceiveNextBit();
};

#endif // LibLanc_NonBlocking_h

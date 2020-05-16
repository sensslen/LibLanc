#ifndef LibLanc_h
#define LibLanc_h

class Lanc
{
public:
  Lanc(uint8_t inputPin, uint8_t outputPin);
  /**
   * Since Interrupts are System global, the program
   * setting up the Lanc protocol must specify a function
   * that handles the pin Interrupt and forwards this call
   * to the class method PinInterrupt.
   */
  void Start(void (*pinHandler)(void), void (*timerHandler)(void));
  void PinInterrupt(void);
  void TimerInterrupt(void);

  /**
   * Set the zoom speed of the camera. 
   * @param stepSize Zoom in or out. The value must be in the range of [-8..8].
   *                 Negative values zoom out. The value of 0 is silently ignored, 
   *                 as it means no zoom and needs no telegram.
   * @retval  true   Zoom command successfully scheduled for transmission to the camera
   * @retval  false  Failed to transmit the command - either the transmission buffer 
   *                 is full or the passed parameter is out of range
   */
  bool Zoom(int8_t stepSize);
  /**
   * Change the manual focus
   * @param far Whether to pusht the Focus farther away or closer
   * @retval  true   Zoom command successfully scheduled for transmission to the camera
   * @retval  false  Failed to transmit the command - either the transmission buffer 
   *                 is full or the passed parameter is out of range
   */
  bool Focus(bool far);
  /**
   * Toggle Autofocus
   * @retval  true   Zoom command successfully scheduled for transmission to the camera
   * @retval  false  Failed to transmit the command - either the transmission buffer 
   *                 is full or the passed parameter is out of range
   */
  bool AutoFocus();

protected:
private:
  uint8_t _inputPin;
  uint8_t _outputPin;

  bool transmitCommandData(uint8_t data);

  struct
  {
    uint8_t data[8];
    uint8_t transmitCount;
  } _currentTransmission;
  struct
  {
    volatile bool pending;
    uint8_t data[8];
  } _pendingTransmission;

  uint8_t _transmitBitCounter;
  uint8_t _transmitbyteCounter;

  unsigned long _lastPinChange;
  volatile uint8_t _currentBit;
  bool _waitForStartBit;
};

#endif // LibLanc_h

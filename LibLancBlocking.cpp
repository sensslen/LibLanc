#include "LibLancBlocking.h"

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#define LANC_BIT_TIME_US (104)
#define LANC_STARTBIT_TIME_US (LANC_BIT_TIME_US)
#define LANC_HALF_BIT_TIME_US ((LANC_BIT_TIME_US) / 2)

#define LANC_VIDEO_CAMERA_SPECIAL_COMMAND 0b00101000

#define transmitIdle transmitZero

LancBlocking::LancBlocking(uint8_t inputPin, uint8_t outputPin) {
  _inputPin = inputPin;
  _outputPin = outputPin;

  memset(_transmitReceiveBuffer, 0, sizeof(_transmitReceiveBuffer));
}

void LancBlocking::begin() {
  pinMode(_inputPin, INPUT);
  pinMode(_outputPin, OUTPUT);
  transmitIdle();
}

void LancBlocking::setTransmitDataVideoCameraSpecialCommand(uint8_t data) {
  _transmitReceiveBuffer[0] = LANC_VIDEO_CAMERA_SPECIAL_COMMAND;
  _transmitReceiveBuffer[1] = data;
}

bool LancBlocking::Zoom(int8_t stepSize) {
  if (stepSize == 0) {
    // we actually want to stop zooming which effectively means
    // clearing the command
    ClearCommand();
    return true;
  }
  if ((stepSize > 8) || (stepSize < -8)) {
    return false;
  }
  if (stepSize > 0) {
    setTransmitDataVideoCameraSpecialCommand((stepSize - 1) * 2);
  } else {
    setTransmitDataVideoCameraSpecialCommand(((-stepSize - 1) * 2) + 0x10);
  }

  return true;
}

void LancBlocking::Focus(bool far) {
  setTransmitDataVideoCameraSpecialCommand((far) ? (0x45) : (0x47));
}

void LancBlocking::AutoFocus() {
  setTransmitDataVideoCameraSpecialCommand(0x41);
}

void LancBlocking::ClearCommand() {
  _transmitReceiveBuffer[0] = 0;
  _transmitReceiveBuffer[1] = 0;
}

void LancBlocking::loop() {
  auto startTime = syncTransmission();

  transmitByte(_transmitReceiveBuffer[0], startTime);
  startTime = waitNextStart();
  transmitByte(_transmitReceiveBuffer[1], startTime);
  startTime = waitNextStart();
  transmitByte(_transmitReceiveBuffer[2], startTime);
  startTime = waitNextStart();
  transmitByte(_transmitReceiveBuffer[3], startTime);
  startTime = waitNextStart();
  receiveByte(&_transmitReceiveBuffer[4], startTime);
  startTime = waitNextStart();
  receiveByte(&_transmitReceiveBuffer[5], startTime);
  startTime = waitNextStart();
  receiveByte(&_transmitReceiveBuffer[6], startTime);
  startTime = waitNextStart();
  receiveByte(&_transmitReceiveBuffer[7], startTime);
}

void LancBlocking::transmitByte(uint8_t byte, unsigned long startTime) {
  waitStartBitComplete(startTime);
  for (uint8_t i = 0; i < 8; i++) {
    if (byte & (1 << i)) {
      transmitOne();
    } else {
      transmitZero();
    }
    delayUsWithStartTime(startTime, (i + 1) * LANC_BIT_TIME_US + LANC_STARTBIT_TIME_US);  // Wait for the bit to be transmitted
  }
}
void LancBlocking::receiveByte(uint8_t *byte, unsigned long startTime) {
  *byte = 0;
  waitStartBitComplete(startTime);
  for (uint8_t i = 0; i < 8; i++) {
    delayUsWithStartTime(startTime, i * LANC_BIT_TIME_US + LANC_HALF_BIT_TIME_US + LANC_STARTBIT_TIME_US);
    if (inputState()) {
      *byte |= 1 << i;
    }
    delayUsWithStartTime(startTime, (i + 1) * LANC_BIT_TIME_US + LANC_STARTBIT_TIME_US);
  }
}

unsigned long LancBlocking::waitNextStart() {
  transmitIdle();                            // make sure to stop current transmission
  delayMicroseconds(LANC_HALF_BIT_TIME_US);  // Make sure to be in the stop bit before waiting for next byte

  return waitForStartBit();
}

void LancBlocking::waitStartBitComplete(unsigned long startTime) {
  delayUsWithStartTime(startTime, LANC_STARTBIT_TIME_US);
}

void LancBlocking::transmitOne() {
  digitalWrite(_outputPin, HIGH);
}

void LancBlocking::transmitZero() {
  digitalWrite(_outputPin, LOW);
}

bool LancBlocking::inputState() {
  return digitalRead(_inputPin);
}

unsigned long LancBlocking::syncTransmission() {
  // Sync to next LANC message
  // lanc protocol requires a stop condition that is longer than 5 milliseconds. Since we expect this function
  // to be called periodically and thus have a delay between completion and next start we use a lower value that
  // is still enough to determine between stop conditions during an ongoing transmission and the stop condition
  // between two transmissions.

  // wait for long enough stop condition
  int stopConditionStart = micros();
  while ((micros() - stopConditionStart) < 3000) {
    if (!inputState()) {
      stopConditionStart = micros();
    }
  }

  return waitForStartBit();
}

void LancBlocking::delayUsWithStartTime(unsigned long startTime, unsigned long waitTime) {
  while ((micros() - startTime) < waitTime) {
    // loop until the delay is waited
  }
}

unsigned long LancBlocking::waitForStartBit() {
  unsigned long startTime = micros();
  while (inputState()) {
    startTime = micros();
  }
  return startTime;
}

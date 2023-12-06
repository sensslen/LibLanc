#include "LibLancNonBlocking.h"

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#define LANC_BIT_TIME_US (104)
#define LANC_HALF_BIT_TIME_US ((LANC_BIT_TIME_US) / 2)
#define LANC_COMPLETE_BYTE_TIME (10 * LANC_BIT_TIME_US)

#define LANC_VIDEO_CAMERA_SPECIAL_COMMAND 0b00101000

#define writeIdle writeZero

LancNonBlocking::LancNonBlocking(uint8_t inputPin, uint8_t outputPin) {
  _inputPin = inputPin;
  _outputPin = outputPin;

  memset(transmitBuffers, 0, sizeof(transmitBuffers));
  currentTransmission = transmitBuffers[0];
  nextTransmission = transmitBuffers[1];
  bufferSwapPending = false;
  currentState = &LancNonBlocking::SearchStart;
  timeStore = 0;
}

void LancNonBlocking::begin() {
  pinMode(_inputPin, INPUT);
  pinMode(_outputPin, OUTPUT);
  writeIdle();
}

void LancNonBlocking::loop() {
  (this->*currentState)();
}

bool LancNonBlocking::Zoom(int8_t stepSize) {
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

void LancNonBlocking::Focus(bool far) {
  setTransmitDataVideoCameraSpecialCommand((far) ? (0x45) : (0x47));
}

void LancNonBlocking::AutoFocus() {
  setTransmitDataVideoCameraSpecialCommand(0x41);
}

void LancNonBlocking::ClearCommand() {
  nextTransmission[0] = 0;
  nextTransmission[1] = 0;
  bufferSwapPending = true;
}

void LancNonBlocking::setTransmitDataVideoCameraSpecialCommand(uint8_t data) {
  nextTransmission[0] = LANC_VIDEO_CAMERA_SPECIAL_COMMAND;
  nextTransmission[1] = data;
  bufferSwapPending = true;
}

void LancNonBlocking::writeOne() {
  digitalWrite(_outputPin, HIGH);
}

void LancNonBlocking::writeZero() {
  digitalWrite(_outputPin, LOW);
}

bool LancNonBlocking::readInput() {
  return digitalRead(_inputPin);
}

int LancNonBlocking::timePassed() {
  return (micros() - timeStore);
}

void LancNonBlocking::swapBuffersIfPending() {
  if (bufferSwapPending) {
    bufferSwapPending = false;
    uint8_t *tempBufferStore = nextTransmission;
    nextTransmission = currentTransmission;
    currentTransmission = tempBufferStore;
  }
}

void LancNonBlocking::SearchStart() {
  if (!readInput()) {
    timeStore = micros();
  }
  if (timePassed() > 3000) {
    currentState = &LancNonBlocking::WaitForTransmissionStart;
  }
}

void LancNonBlocking::WaitForTransmissionStart() {
  if (!readInput()) {
    currentBit = 0;
    timeStore = micros();
    currentState = &LancNonBlocking::WaitToTransmitNextBit;
  }
}

void LancNonBlocking::WaitToTransmitNextBit() {
  uint8_t bitNr = currentBit % 8;

  // calculate delay from start bit in order to avoid adding up errors
  if (timePassed() >= (bitNr + 1) * LANC_BIT_TIME_US) {
    if (TransmitNextBit()) {
      currentState = &LancNonBlocking::WaitToTransmitStopBit;
    }
  }
}

void LancNonBlocking::WaitToTransmitStopBit() {
  // calculate delay from start bit in order to avoid adding up errors
  if (timePassed() >= (8 + 1) * LANC_BIT_TIME_US) {
    writeIdle();
    currentState = &LancNonBlocking::WaitForNextStartBit;
  }
}

void LancNonBlocking::WaitForNextStartBit() {
  // make sure to only start searching for the start condition when the stop
  // condition is at least half way through
  if ((timePassed() > (LANC_COMPLETE_BYTE_TIME - LANC_HALF_BIT_TIME_US)) && !readInput()) {
    timeStore = micros();
    if (currentBit >= (2 * 8)) {
      currentState = &LancNonBlocking::WaitToReceiveNextBit;
    } else {
      currentState = &LancNonBlocking::WaitToTransmitNextBit;
    }
  }
}

void LancNonBlocking::WaitToReceiveNextBit() {
  uint8_t bitNr = currentBit % 8;

  // calculate delay from start bit in order to avoid adding up errors
  if (timePassed() >= (((bitNr + 1) * LANC_BIT_TIME_US) + LANC_HALF_BIT_TIME_US)) {
    if (ReceiveNextBit()) {
      if (currentBit >= 8 * 8) {
        swapBuffersIfPending();
        currentState = &LancNonBlocking::SearchStart;
      } else {
        currentState = &LancNonBlocking::WaitForNextStartBit;
      }
    }
  }
}

bool LancNonBlocking::TransmitNextBit() {
  uint8_t byte = currentBit / 8;
  uint8_t bit = currentBit % 8;

  if (currentTransmission[byte] & (1 << bit)) {
    writeOne();
  } else {
    writeZero();
  }

  currentBit++;
  return bit == 7;
}

bool LancNonBlocking::ReceiveNextBit() {
  uint8_t byte = currentBit / 8;
  uint8_t bit = currentBit % 8;

  if (readInput()) {
    currentTransmission[byte] |= 1 << bit;
  }

  currentBit++;
  return bit == 7;
}

#include "LibLancBlocking.h"

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#define LANC_BIT_TIME_US (104)
#define LANC_HALF_BIT_TIME_US ((LANC_BIT_TIME_US) / 2)
// LANC_BLOCKED_CALL_TIMEOUT_MS
//      This is the function timeout,  in MS.   If this timeout is exceeded the library will disable
//      itself to allow control back to the calling program.  Calling begin() will start everything again.
#define LANC_BLOCKED_CALL_TIMEOUT_MS (100)
bool LANC_FUNCTION_LOCKOUT = false;
#define LANC_VIDEO_CAMERA_SPECIAL_COMMAND 0b00101000
#define LANC_VIDEO_CAMERA_NORMAL_COMMAND 0b00011000
#define LANC_VIDEO_CAMERA_OPS_COMMAND 0b11011000

LancBlocking::LancBlocking(uint8_t inputPin, uint8_t outputPin)
{
    _inputPin = inputPin;
    _outputPin = outputPin;
    LANC_FUNCTION_LOCKOUT = false; // As Library is initialized the function lockout begins as disabled.

    memset(_transmitReceiveBuffer, 0, sizeof(_transmitReceiveBuffer));
}

void LancBlocking::begin()
{
    pinMode(_inputPin, INPUT);
    pinMode(_outputPin, OUTPUT);
    LANC_FUNCTION_LOCKOUT = false; // Clears the lockout (If it was enabled), which allows the calling program to start transmitting again.
    ClearCommand();
    transmitZero();
}

void LancBlocking::setTransmitDataVideoCameraSpecialCommand(uint8_t data)
{
    _transmitReceiveBuffer[0] = LANC_VIDEO_CAMERA_SPECIAL_COMMAND;
    _transmitReceiveBuffer[1] = data;
}

void LancBlocking::setTransmitDataVideoCameraNormalCommand(uint8_t data)
{
    _transmitReceiveBuffer[0] = LANC_VIDEO_CAMERA_NORMAL_COMMAND;
    _transmitReceiveBuffer[1] = data;
}
void LancBlocking::setTransmitDataVideoCameraOpsCommand(uint8_t data)
{
    _transmitReceiveBuffer[0] = LANC_VIDEO_CAMERA_OPS_COMMAND;
    _transmitReceiveBuffer[1] = data;
}

bool LancBlocking::Zoom(int8_t stepSize)
{
    ClearCommand();
    if (stepSize == 0)
    {
        // we actually want to stop zooming which effectively means
        // clearing the command
        ClearCommand();
        return true;
    }
    if ((stepSize > 8) || (stepSize < -8))
    {
        return false;
    }
    if (stepSize > 0)
    {
        setTransmitDataVideoCameraSpecialCommand((stepSize - 1) * 2);
    }
    else
    {
        setTransmitDataVideoCameraSpecialCommand(((-stepSize - 1) * 2) + 0x10);
    }

    return true;
}
void LancBlocking::RecordToggle()
{
    ClearCommand();
    setTransmitDataVideoCameraNormalCommand(0x33);
}
void LancBlocking::PowerDown()
{
    ClearCommand();
    setTransmitDataVideoCameraNormalCommand(0x5e);
}
void LancBlocking::Focus(bool far)
{
    ClearCommand();
    setTransmitDataVideoCameraSpecialCommand((far) ? (0x45) : (0x47));
}
void LancBlocking::Iris(bool moreopen)
{
    ClearCommand();
    setTransmitDataVideoCameraSpecialCommand((moreopen) ? (0x55) : (0x53));
}
void LancBlocking::AutoIris()
{
    ClearCommand();
    setTransmitDataVideoCameraSpecialCommand(0xAF);
}
void LancBlocking::AutoFocus()
{
    ClearCommand();
    setTransmitDataVideoCameraSpecialCommand(0x41);
}
void LancBlocking::VideoCameraSpecialCommand(uint8_t byte)
{
    ClearCommand();
    setTransmitDataVideoCameraSpecialCommand(byte);
}
void LancBlocking::VideoCameraNormalCommand(uint8_t byte)
{
    ClearCommand();
    setTransmitDataVideoCameraNormalCommand(byte);
}
void LancBlocking::VideoCameraOpsCommand(uint8_t byte)
{
    ClearCommand();
    setTransmitDataVideoCameraOpsCommand(byte);
}
void LancBlocking::ClearCommand()
{
    // Clear the buffers
    _transmitReceiveBuffer[0] = 0;
    _transmitReceiveBuffer[1] = 0;
}

bool LancBlocking::loop()
{
    // If the lockout variable is set then exit out of the loop with false (So the calling program can take action)
    if (LANC_FUNCTION_LOCKOUT)
    {
        return false;
    }

    // If the syncTrasmission fails we likely have a hardware failure.  The lockout will be set, return false to
    //  pass control back to calling program, preventing full software blocking.
    auto startTime = syncTransmission();
    if (!startTime)
    {
        return false;
    }

    // Start transmitting and receving data, any failures will result in lockout variable being set and returning false.
    if (!transmitByte(_transmitReceiveBuffer[0], startTime))
    {
        return false;
    };
    startTime = waitForStartBit();
    if (!transmitByte(_transmitReceiveBuffer[1], startTime))
    {
        return false;
    };
    startTime = waitForStartBit();
    if (!receiveByte(&_transmitReceiveBuffer[2], startTime))
    {
        return false;
    };
    startTime = waitForStartBit();
    if (!receiveByte(&_transmitReceiveBuffer[3], startTime))
    {
        return false;
    };
    startTime = waitForStartBit();
    if (!receiveByte(&_transmitReceiveBuffer[4], startTime))
    {
        return false;
    };
    startTime = waitForStartBit();
    if (!receiveByte(&_transmitReceiveBuffer[5], startTime))
    {
        return false;
    };
    startTime = waitForStartBit();
    if (!receiveByte(&_transmitReceiveBuffer[6], startTime))
    {
        return false;
    };
    startTime = waitForStartBit();
    if (!receiveByte(&_transmitReceiveBuffer[7], startTime))
    {
        return false;
    };

    return true; // loop completed without issue,  return true.
}

bool LancBlocking::transmitByte(uint8_t byte, unsigned long startTime)
{
    transmitZero();
    for (uint8_t i = 0; i < 8; i++)
    {
        if (byte & (1 << i))
        {
            transmitOne();
        }
        else
        {
            transmitZero();
        }
        delayUsWithStartTime(startTime, (i + 1) * LANC_BIT_TIME_US); // Wait for the bit to be transmitted
    }

    // Check if the total time to send the data is within tollerance.  If not set lockout and return false.
    //  This should not really ever occur unless microcontroller is running to slow.
    unsigned long ttime = micros() - startTime;
    if (ttime >= ((8 * LANC_BIT_TIME_US) + LANC_HALF_BIT_TIME_US))
    {
        LANC_FUNCTION_LOCKOUT = true;
        ClearCommand();
        return false;
    }

    transmitZero(); // Complete transmission
    return true;
}
bool LancBlocking::receiveByte(uint8_t *byte, unsigned long startTime)
{
    *byte = 0;
    for (uint8_t i = 0; i < 8; i++)
    {
        if (inputState())
        {
            *byte |= 1 << i;
        }
        delayUsWithStartTime(startTime, (i + 1) * LANC_BIT_TIME_US);
    }

    // Check if the total time to receive the data is within tollerance.  If not set lockout and return false.
    //  This should not really ever occur unless microcontroller is running to slow.
    unsigned long ttime = micros() - startTime;
    if (ttime >= ((8 * LANC_BIT_TIME_US) + LANC_HALF_BIT_TIME_US))
    {
        LANC_FUNCTION_LOCKOUT = true;
        ClearCommand();
        return false;
    }
    return true;
}

void LancBlocking::transmitOne()
{
    digitalWrite(_outputPin, HIGH);
}

void LancBlocking::transmitZero()
{
    digitalWrite(_outputPin, LOW);
}

bool LancBlocking::inputState()
{
    return digitalRead(_inputPin);
}

// Sync to next LANC message
// lanc protocol requires a stop condition that is longer than 5 milliseconds. Since we expect this function
// to be called periodically and thus have a delay between completion and next start we use a lower value that
// is still enough to determine between stop conditions during an ongoing transmission and the stop condition
// between two transmissions.
unsigned long LancBlocking::syncTransmission()
{
    bool bool_wait_for_start_bit = true;                   // Once the bool is set false the function believes we have located the end of the first start bit and we are ready to transmit commands.  The current time (us) is retured from the function.
    unsigned long pulse_transition_duration = 0;           //  The variable will be used to hold the total time (us) from when the input pin moves from high to low and high again to evaluate if we are at the end of the first start bit or in the middle of a transaction
    unsigned long wait_high_startTime = micros();          // set the start time (us) to monitor the input pin transition from high to low
    unsigned long wait_low_startTime = micros();           // set the start time (us) to monitor the input pin transition from low to high
    unsigned long lockout_evaluation_startTime = millis(); // Set the start time (ms) that the smaller block of code was started, used to evaluate if we exceed the max value and are stuck in a loop while waiting for pin states to go from high to low and low to high.
    unsigned long syncTransmission_startTime = millis();   // Set the start time (ms) that the syncTransmission function was called to know if we exceed the total allowed time for the function to complete.
    bool pin_status = HIGH;                                // A default state,  will be overwrote before used.
    while (bool_wait_for_start_bit)
    {
        // Wait for input to move from HIGH to LOW
        // ***************
        if (syncTransmission_startTime - LANC_BLOCKED_CALL_TIMEOUT_MS < 0)
        {
            syncTransmission_startTime = millis();
        } // Account for internal clock rollover
        lockout_evaluation_startTime = millis();
        pin_status = inputState();
        wait_high_startTime = micros();
        while (pin_status)
        {
            if (lockout_evaluation_startTime - LANC_BLOCKED_CALL_TIMEOUT_MS < 0)
            {
                lockout_evaluation_startTime = millis();
            } // Account for clock rollover
            if (millis() - lockout_evaluation_startTime >= LANC_BLOCKED_CALL_TIMEOUT_MS)
            {
                LANC_FUNCTION_LOCKOUT = true;
                ClearCommand();
                return false;
                // Time to disable the blocking until external issue is resolved.
            }
            pin_status = inputState();
        }
        // ***************

        // Wait for input to move from LOW to HIGH
        // ***************
        lockout_evaluation_startTime = millis();
        wait_low_startTime = micros();
        while (!pin_status)
        {
            if (lockout_evaluation_startTime - LANC_BLOCKED_CALL_TIMEOUT_MS < 0)
            {
                lockout_evaluation_startTime = millis();
            } // Account for internal clock rollover
            if (millis() - lockout_evaluation_startTime >= LANC_BLOCKED_CALL_TIMEOUT_MS)
            {
                LANC_FUNCTION_LOCKOUT = true;
                ClearCommand();
                return false;
                // Time to disable the blocking until external issue is resolved.
            }
            pin_status = inputState();
        }
        // ***************

        // Evaluate if we have exceeded the total allowed time for this sync to run.  If exceeded lockout and return false.
        long elapsed = millis() - syncTransmission_startTime;
        if (elapsed >= (LANC_BLOCKED_CALL_TIMEOUT_MS))
        {
            LANC_FUNCTION_LOCKOUT = true;
            ClearCommand();
            return false;
            // Time to disable the blocking until external issue is resolved.
        }

        // Evaluate if the transition state duration suggests we are at the end of the first start bit,
        //  This indicates to us we are ready to begin transmitting data.  If so return the current time in microseconds.
        pulse_transition_duration = wait_low_startTime - wait_high_startTime;
        if (pulse_transition_duration > 3000)
        {
            bool_wait_for_start_bit = false;
            return micros();
        }
    }
    return micros();
}

void LancBlocking::delayUsWithStartTime(unsigned long startTime, unsigned long waitTime)
{
    while ((micros() - startTime) < waitTime)
    {
        // loop until the delay is waited
    }
}

// This function simply waits for the next high->low->high transition,  its assumed you are calling this during the stop bits of the transaction.  No timing check outside of global lockout is evaulated.  Returns current time (us) once function passed
unsigned long LancBlocking::waitForStartBit()
{
    unsigned long lockout_evaluation_startTime = millis();
    bool pin_status = HIGH;
    lockout_evaluation_startTime = millis();
    pin_status = inputState();
    while (pin_status)
    {
        if (lockout_evaluation_startTime - LANC_BLOCKED_CALL_TIMEOUT_MS < 0)
        {
            lockout_evaluation_startTime = millis();
        } // Account for internal clock rollover
        if (millis() - lockout_evaluation_startTime >= LANC_BLOCKED_CALL_TIMEOUT_MS)
        {
            LANC_FUNCTION_LOCKOUT = true;
            ClearCommand();
            return false;
            // Time to disable the blocking until external issue is resolved.
        }
        pin_status = inputState();
    }
    lockout_evaluation_startTime = millis();
    while (!pin_status)
    {
        if (lockout_evaluation_startTime - LANC_BLOCKED_CALL_TIMEOUT_MS < 0)
        {
            lockout_evaluation_startTime = millis();
        } // Account for internal clock rollover
        if (millis() - lockout_evaluation_startTime >= LANC_BLOCKED_CALL_TIMEOUT_MS)
        {
            LANC_FUNCTION_LOCKOUT = true;
            ClearCommand();
            return false;
            // Time to disable the blocking until external issue is resolved.
        }
        pin_status = inputState();
    }
    return micros();
}
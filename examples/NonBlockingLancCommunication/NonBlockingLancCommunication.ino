#include <LibLanc.h>

// LibLanc
// by Simon Ensslen <https://github.com/sensslen>
// This example illustrates the usage of the liblanc library in blocking mode

#define LANC_INPUT_PIN 2
#define LANC_OUTPUT_PIN 3
LancNonBlocking lanc(LANC_INPUT_PIN, LANC_OUTPUT_PIN);
String receivedCommand = "";

void setup() {
  lanc.begin();
  Serial.begin(9600);

  while (!Serial) {
    ;  // wait for serial port to connect. Needed for native USB
  }

  Serial.println("<Arduino is ready>");
}

void loop() {
  // get next command to execute
  checkCommand();

  // call loop as fast as possible
  // to not loose track of the lanc communication
  lanc.loop();
}

void checkCommand() {
  while (Serial.available() > 0) {
    char read = Serial.read();
    if (read == '\n') {
      lanc.Zoom(receivedCommand.toInt());
      receivedCommand = "";
      break;
    } else {
      receivedCommand += read;
    }
  }
}

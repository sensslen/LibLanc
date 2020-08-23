#include <LibLanc.h>

// LibLanc
// by Simon Ensslen <https://github.com/sensslen>

#define LANC_INPUT_PIN  2
#define LANC_OUTPUT_PIN  3
Lanc lanc(LANC_INPUT_PIN, LANC_OUTPUT_PIN);

void setup() {
    lanc.begin();
}

void loop() {
    // get next command to execute
    // call lanc.Zoom(value);
    lanc.loop();
}


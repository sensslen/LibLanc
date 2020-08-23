# LibLanc

This Arudino library allows to control a [Sony Lanc](http://www.boehmel.de/lanc) enabled camera. Since the Protocol has very strict timing requirements, the library is designed so that it blocks until a complete communication exchange is completed. This is triggered by calling the `loop` function.

## Example usage

The following example shows how LibLanc could be used:

```c++
#include <LibLanc.h>

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
```

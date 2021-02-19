# LibLanc

This Arudino library allows to control a [Sony Lanc](http://www.boehmel.de/lanc) enabled camera.
Since the Protocol has very strict timing requirements, the library does provide a blocking option which should be used normally. It is designed so that it blocks until a complete communication exchange is completed. This is triggered by calling the `loop` function.

There is a non blocking variant of the Library available (by using `LancNonBlocking` class). be aware that you must call `loop` as quickly as possible. It is recommended to call `loop` at least every 25us.

## Example usage

The following example shows how LibLanc could be used:

```c++
#include <LibLanc.h>

#define LANC_INPUT_PIN  2
#define LANC_OUTPUT_PIN  3
LancBlocking lanc(LANC_INPUT_PIN, LANC_OUTPUT_PIN);

void setup() {
    lanc.begin();
}

void loop() {
    // get next command to execute
    // call lanc.Zoom(value);
    lanc.loop();
}
```

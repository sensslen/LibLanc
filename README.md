# LibLanc [![CodeFactor](https://www.codefactor.io/repository/github/sensslen/liblanc/badge)](https://www.codefactor.io/repository/github/sensslen/liblanc)

This Arudino library allows to control a [Sony Lanc](http://www.boehmel.de/lanc) enabled camera.

Since the Protocol has very strict timing requirements, the application should normally block during transmission (see `LancBlocking` class). This class is designed so that it loops until a complete communication exchange is completed. This is triggered by calling the `loop` function.

There is a non blocking variant of the Library available (see `LancNonBlocking` class). This variant simply checks whether there is anything to do and immediately returns. Be aware that you must call `loop` very quickly. It is recommended to call `loop` at least every 25us.

## Example usage

The following example shows how `LancBlocking` could be used:

```c++
#include <Commands/ZoomCommand.h>
#include <LibLanc.h>

#include <memory>

#define LANC_INPUT_PIN 2
#define LANC_OUTPUT_PIN 3
LibLanc::LancBlocking lanc(LANC_INPUT_PIN, LANC_OUTPUT_PIN);

void setup()
{
    lanc.begin();
}

void loop()
{
    // get next command to execute
    // call lanc.setCommand(LibLanc::CommandFactory::zoom(3));
    lanc.loop();
}
```

For more examples see: [examples](examples)

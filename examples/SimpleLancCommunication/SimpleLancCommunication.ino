#include <Commands/ZoomCommand.h>
#include <LibLanc.h>

#include <memory>

// LibLanc
// by Simon Ensslen <https://github.com/sensslen>
// This example illustrates the usage of the liblanc library in blocking mode

#define LANC_INPUT_PIN 2
#define LANC_OUTPUT_PIN 3
LancBlocking lanc(LANC_INPUT_PIN, LANC_OUTPUT_PIN);

void setup()
{
    lanc.begin();
}

void loop()
{
    // get next command to execute
    // call lanc.setCommand(std::make_shared<LibLanc::Commands::ZoomCommand>(3));
    lanc.loop();
}

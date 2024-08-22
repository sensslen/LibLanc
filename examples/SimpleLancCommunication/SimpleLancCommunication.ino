#include <Commands/ZoomCommand.h>
#include <LibLanc.h>

#include <memory>

// LibLanc
// by Simon Ensslen <https://github.com/sensslen>
// This example illustrates the usage of the liblanc library in blocking mode

#define LANC_INPUT_PIN 2
#define LANC_OUTPUT_PIN 3
std::unique_ptr<LibLanc::App::Lanc> lanc;

void setup()
{
    LibLanc::LancBuilder lancBuilder;
    lancBuilder.UseTwoPinPhysicalLayer(LANC_INPUT_PIN, LANC_OUTPUT_PIN, LibLanc::Phy::OutputType::PushPull);
    lanc = lancBuilder.CreateBlocking();
    lanc->begin();
}

void loop()
{
    // get next command to execute
    // call lanc.setCommand(LibLanc::CommandFactory::zoom(3));
    lanc->loop();
}

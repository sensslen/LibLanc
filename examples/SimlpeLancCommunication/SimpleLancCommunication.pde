#include <LibLanc.h>

// CommunicateLanc
// by Simon Ensslen <https://github.com/sensslen>

// Demostrates how to do something with the Test library

Lanc myLancCommunicator = Lanc();

void setup()
{
   myLancCommunicator.Start();
}

void loop()
{
  myLancCommunicator.Zoom(20);
  delay(500);
}


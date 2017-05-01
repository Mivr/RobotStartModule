// Example
// Here you can see how to use the library for the starter module.
// NOTE: Do not forget to set the pin to OUTPUT as the LED will require power
// Schematic:
// Connect an IR led to pin 9 and GND (longer leg goes to pin 9, shorter to GND)

#include <StarterRemote.h>

StarterRemote myStarter(9);

void setup()
{
  pinMode(9, OUTPUT);
}

void loop()
{
  myStarter.sendReprogramCommand(10);
  delay( 5000 );
  myStarter.sendStartCommand(10);
  delay( 4000 );
  myStarter.sendStopCommand(10);
  delay( 20000 );
}

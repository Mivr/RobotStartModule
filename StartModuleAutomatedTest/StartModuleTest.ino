
#define START_MODULE_START_PIN  5
#define SIGNAL_LED_PIN          6
#define START_MODULE_KILL_PIN   7
#define START_MODULE_POWER_PIN  8
#define REMOTE_LED_PIN          9
#define START_TEST_BUTTON_PIN   10

#define REMOTE_ADDRESS          0x12

#include "StarterRemote.h"

StarterRemote * remote;

void setup()
{
  while (!SerialUSB) ;
  SerialUSB.begin(115200);
  pinMode(SIGNAL_LED_PIN, OUTPUT);
  
  pinMode(START_MODULE_POWER_PIN, OUTPUT);
  pinMode(START_MODULE_KILL_PIN,  INPUT_PULLDOWN);
  pinMode(START_MODULE_START_PIN, INPUT_PULLDOWN);
  pinMode(START_TEST_BUTTON_PIN,  INPUT_PULLUP);

  remote = new StarterRemote(REMOTE_LED_PIN);
  
  digitalWrite(START_MODULE_POWER_PIN, LOW);
}

uint8_t errors = 0;

void loop()
{
  digitalWrite(SIGNAL_LED_PIN, HIGH);
  
  while(digitalRead(START_TEST_BUTTON_PIN) == HIGH);

  errors = 0;
  
  digitalWrite(START_MODULE_POWER_PIN, HIGH);
  digitalWrite(SIGNAL_LED_PIN, LOW);

  delay(200);

  SerialUSB.println("Start test of attached module");

  if(digitalRead(START_MODULE_KILL_PIN) != HIGH)
  {
    SerialUSB.println("ERROR ID 001 Comment: KILL pin is LOW after powering up the module");
    errors++;
  }
  if(digitalRead(START_MODULE_START_PIN) != LOW)
  {
    SerialUSB.println("ERROR ID 002 Comment: START pin is HIGH after powering up the module (we expect a not started module)");
    errors++;
  }

  // reprogram module to respond to remote

  SerialUSB.println("1. Sending program command");
  remote->sendReprogramCommand(REMOTE_ADDRESS);

  delay(10);

  if(digitalRead(START_MODULE_KILL_PIN) != HIGH)
  {
    SerialUSB.println("ERROR ID 003 Comment: KILL pin is LOW after programming was send");
    errors++;
  }
  if(digitalRead(START_MODULE_START_PIN) != LOW)
  {
    SerialUSB.println("ERROR ID 004 Comment: START pin is HIGH after programming was send");
    errors++;
  }

  delay(3000);

  // send start command

  SerialUSB.println("2. Sending start command");
  remote->sendStartCommand(REMOTE_ADDRESS);

  delay(10);

  if(digitalRead(START_MODULE_KILL_PIN) != HIGH)
  {
    SerialUSB.println("ERROR ID 005 Comment: KILL pin is LOW after module was started");
    errors++;
  }
  if(digitalRead(START_MODULE_START_PIN) != HIGH)
  {
    SerialUSB.println("ERROR ID 006 Comment: START pin is LOW after module was started");
    errors++;
  }

  delay(1000);

  // power off during started state

  SerialUSB.println("3. Powering off the module");
  digitalWrite(START_MODULE_POWER_PIN, LOW);

  delay(10);

  if(digitalRead(START_MODULE_KILL_PIN) != LOW)
  {
    SerialUSB.println("ERROR ID 007 Comment: KILL pin is HIGH when module is shutdown");
    errors++;
  }
  if(digitalRead(START_MODULE_START_PIN) != LOW)
  {
    SerialUSB.println("ERROR ID 008 Comment: START pin is HIGH when module is shutdown");
    errors++;
  }

  delay(1000);

  // power up the module

  SerialUSB.println("4. Powering on the module");
  digitalWrite(START_MODULE_POWER_PIN, HIGH);

  delay(200);

  if(digitalRead(START_MODULE_KILL_PIN) != HIGH)
  {
    SerialUSB.println("ERROR ID 009 Comment: KILL pin is LOW after module was powered up in started state");
    errors++;
  }
  if(digitalRead(START_MODULE_START_PIN) != HIGH)
  {
    SerialUSB.println("ERROR ID 010 Comment: START pin is LOW after module was powered up in started state");
    errors++;
  }

  delay(1000);

  // stop the module

  SerialUSB.println("5. Stopping the module");
  remote->sendStopCommand(REMOTE_ADDRESS);

  delay(10);

  if(digitalRead(START_MODULE_KILL_PIN) != LOW)
  {
    SerialUSB.println("ERROR ID 011 Comment: KILL pin is HIGH after module was stopped");
    errors++;
  }
  if(digitalRead(START_MODULE_START_PIN) != LOW)
  {
    SerialUSB.println("ERROR ID 012 Comment: START pin is HIGH after module was stopped");
    errors++;
  }

  delay(2000);

  // power down the module

  SerialUSB.println("6. Powering off the module");
  digitalWrite(START_MODULE_POWER_PIN, LOW);

  delay(10);

  if(digitalRead(START_MODULE_KILL_PIN) != LOW)
  {
    SerialUSB.println("ERROR ID 013 Comment: KILL pin is HIGH when module is shutdown after stop");
    errors++;
  }
  if(digitalRead(START_MODULE_START_PIN) != LOW)
  {
    SerialUSB.println("ERROR ID 014 Comment: START pin is HIGH when module is shutdown after stop");
    errors++;
  }

  delay(1000);

  // power up the module

  SerialUSB.println("7. Powering on the module");
  digitalWrite(START_MODULE_POWER_PIN, HIGH);

  delay(200);

  if(digitalRead(START_MODULE_KILL_PIN) != HIGH)
  {
    SerialUSB.println("ERROR ID 015 Comment: KILL pin is LOW after module was powered up in initial state");
    errors++;
  }
  if(digitalRead(START_MODULE_START_PIN) != LOW)
  {
    SerialUSB.println("ERROR ID 016 Comment: START pin is HIGH after module was powered up in initial state");
    errors++;
  }

  delay(1000);

  // start the module again without reprogramming
  
  SerialUSB.println("8. Starting the module");
  remote->sendStartCommand(REMOTE_ADDRESS);

  delay(10);

  if(digitalRead(START_MODULE_KILL_PIN) != HIGH)
  {
    SerialUSB.println("ERROR ID 017 Comment: KILL pin is LOW after module was started");
    errors++;
  }
  if(digitalRead(START_MODULE_START_PIN) != HIGH)
  {
    SerialUSB.println("ERROR ID 018 Comment: START pin is LOW after module was started");
    errors++;
  }

  delay(1000);

  // stop the module

  SerialUSB.println("9. Stopping the module");
  remote->sendStopCommand(REMOTE_ADDRESS);

  delay(10);

  if(digitalRead(START_MODULE_KILL_PIN) != LOW)
  {
    SerialUSB.println("ERROR ID 019 Comment: KILL pin is HIGH after module was stopped");
    errors++;
  }
  if(digitalRead(START_MODULE_START_PIN) != LOW)
  {
    SerialUSB.println("ERROR ID 020 Comment: START pin is HIGH after module was stopped");
    errors++;
  }

  delay(2000);

  // power down the module

  SerialUSB.println("10. Powering off the module");
  digitalWrite(START_MODULE_POWER_PIN, LOW);

  delay(10);

  if(digitalRead(START_MODULE_KILL_PIN) != LOW)
  {
    SerialUSB.println("ERROR ID 021 Comment: KILL pin is HIGH when module is shutdown");
    errors++;
  }
  if(digitalRead(START_MODULE_START_PIN) != LOW)
  {
    SerialUSB.println("ERROR ID 022 Comment: START pin is HIGH when module is shutdown");
    errors++;
  }

  delay(1000);

  SerialUSB.print("Error occured: ");
  SerialUSB.println(errors);
  if(errors == 0)SerialUSB.println("TEST OK");
  else SerialUSB.println("TEST FAILED");
}
#define LED_PIN 13
#define START_PIN 2

#define BLINK_INTERVAL 100

uint32_t lastTimeLedWasUpdated = 0;
uint8_t newLedState = 0;

void setup()
{
    pinMode(LED_PIN, OUTPUT);
    pinMode(START_PIN, INPUT);
}

void loop()
{
    // in case we have a START signal from the module (we need to be running)
    if(HIGH == digitalRead(START_PIN))
    {
        // The following is a blink without delay implementation
        // you should place your logic here, and remove all delay from it

        // if more more than the time for blinking has passed
        if (lastTimeLedWasUpdated + BLINK_INTERVAL < millis())
        {
            // we go to the next state of the pin
            digitalWrite(LED_PIN, newLedState);

            // we set the next state to be the opposite of the current state of the LED
            newLedState = !newLedState;
            
            // we remember the last time we updated the LED state
            lastTimeLedWasUpdated = millis();
        }
    }
    // in case we have a STOP signal from the module
    else
    {
        // set the LED to OFF
        digitalWrite(LED_PIN, LOW);
    }
}

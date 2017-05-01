/*
 * ledDriver.h
 *
 * Created: 3/25/2017 3:50:45 PM
 *  Author: MIVR
 */ 


#ifndef LEDDRIVER_H_
#define LEDDRIVER_H_

// Set timer to 'Fast PWM' mode. Used for user LED
// set the PWM duty to ~4%
// shut down the LED PWM
#define LED_DRIVER_INITIALIZE \
	TCCR0A |= (1 << WGM01) | (1 << WGM00); \
	OCR0A = 10; \
	TCCR0A &= ~(1 << COM0A1);				


// start the LED PWM on OUTPUT A
#define LED_DRIVER_POWER_ON_LED TCCR0A |= (1 << COM0A1)

// shut down the LED PWM
#define LED_DRIVER_POWER_OFF_LED TCCR0A &= ~(1 << COM0A1)

// led blink sequence for programming
#define LED_DRIVER_BLINK_LED_PROGRAMMING \
LED_DRIVER_POWER_ON_LED;\
_delay_ms(500);\
LED_DRIVER_POWER_OFF_LED;\
_delay_ms(500);\
LED_DRIVER_POWER_ON_LED;\
_delay_ms(500);\
LED_DRIVER_POWER_OFF_LED

// led blink sequence for deactivated
#define LED_DRIVER_BLINK_LED_DEACTIVATED \
while(1)\
{\
	TCCR0A |= (1 << COM0A1);\
	_delay_ms(1000);\
	TCCR0A &= ~(1 << COM0A1);\
	_delay_ms(1000);\
}

#endif /* LEDDRIVER_H_ */
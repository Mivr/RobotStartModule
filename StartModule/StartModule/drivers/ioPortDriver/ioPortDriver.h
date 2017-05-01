/*
 * ioPortDriver.h
 *
 * Created: 3/25/2017 3:27:39 PM
 *  Author: MIVR
 */ 


#ifndef IOPORTDRIVER_H_
#define IOPORTDRIVER_H_

#include "stdint.h"

// IO Pins
#define DO_LED_PIN						0	// OUTPUT pin used for LED output HIGH is ON LOW is OFF PWM is used over this IO PIN
#define DO_START_PIN					2	// OUTPUT pin used for control of user MCU. Indicates start with HIGH level and stop with LOW level
#define DO_KILL_SWITCH_PIN				1	// OUTPUT pin used for control of user kill switch. HIGH is INACTIVE LOW is ACTIVE

#define PORT_STATE_IDLE					0
#define PORT_STATE_ACTIVE				1
#define PORT_STATE_DEACTIVATED			2

#define PORT_DRIVER_INITIALIZE()		DDRB = (1 << DO_LED_PIN) | (1 << DO_KILL_SWITCH_PIN) | (1 << DO_START_PIN)

#define PORT_DRIVER_SET_STATE_IDLE() \
	PORTB = (1 << DO_KILL_SWITCH_PIN)
	
#define PORT_DRIVER_SET_STATE_ACTIVE() \
	PORTB = (1 << DO_START_PIN) | (1 << DO_KILL_SWITCH_PIN)

#define PORT_DRIVER_SET_STATE_DEACTIVATED() \
	PORTB = 0;

#endif /* IOPORTDRIVER_H_ */
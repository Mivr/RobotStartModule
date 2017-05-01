// main.c
//
// Created: 9/11/2016 9:56:26 PM
// Author : MIVR
 

#define F_CPU	9600000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include "ir.h"
#include "drivers/ioPortDriver/ioPortDriver.h"
#include "drivers/ledDriver/ledDriver.h"
#include "drivers/eepromDriver/eepromDriver.h"
#include "stdint.h"

// States of main logic state machine
#define IDLE				1
#define ACTIVE				2
#define DEACTIVATED			3

//////////////////////////////////////////////////////////////////////////
//					Local variables
//////////////////////////////////////////////////////////////////////////
static uint8_t currentState = IDLE, currentStopCommand = 0x00;
static uint16_t secondaryStartCommand = 0x00, secondaryStopCommand = 0x00;

//////////////////////////////////////////////////////////////////////////
//					LOCAL FUNCTIONS
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//	NAME:	main
//
//	DESCRIPTION:	Main function of the program. Will be ran on POR.
//
//	ATRIBUTES:		-
//
//	RETURN VALUE:	-
//
//////////////////////////////////////////////////////////////////////////
int main()
{
	// initialize port driver
	PORT_DRIVER_INITIALIZE();
	
	// initialize led driver
	LED_DRIVER_INITIALIZE;
	
	// load the EEPROM data in RAM mirror provided
	EEPROM_INITIALIZE(&currentState, &currentStopCommand);
	
	// load secondary data for start and stop
	EEPROM_INITIALIZE_SECONDARY(&secondaryStartCommand, &secondaryStopCommand);
	
	// initialize the IR receiver sub module
	IR_INITIALIZE;

	// check start state read from EEPROM
	// in case the saved state was ACTIVE (we lost power during active fight)
	// we need to set the ports state and LED states accordingly
	if (ACTIVE == currentState)
	{
		PORT_DRIVER_SET_STATE_ACTIVE();
		
		LED_DRIVER_POWER_ON_LED;
		
		currentState = ACTIVE;
	}
	// in any other case we go to IDLE state
	else
	{
		// in case we need to go to IDLE state set the port state accordingly
		PORT_DRIVER_SET_STATE_IDLE();
		
		// LED state is OFF from INIT so no need to set it explicitly
		
		// we need to set the state of the state explicitly as other code depends on this state
		currentState = IDLE;
	}
	
    // start interrupts (initialization phase ended)
	sei();
	
	while(1)
	{
	}
}

//////////////////////////////////////////////////////////////////////////
//					GLOBAL FUNCTIONS
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//	NAME:	handleEvent_dataReceivedOnIR
//
//	DESCRIPTION:	Event receiver from IR driver. It handles
//					state transitions and actions for the event.
//
//	ATRIBUTES:
//				uint8_t addressReceived
//					- INPUT the address received from the IR driver
//
//				uint8_t dataReceived
//					- INPUT the data received from the IR driver
//
//	RETURN VALUE:	-
//
//////////////////////////////////////////////////////////////////////////
void handleEvent_dataReceivedOnIR(uint16_t dataReceived)
{
	// reprogramming command received
	if (dataReceived & 0x3f == RC5_ADR_PROGRAMMING && (IDLE == currentState))
	{
		currentStopCommand = dataReceived & 0b11111110;
		
		EEPROM_WRITE_STOP_COMMAND(currentStopCommand);
		
		LED_DRIVER_BLINK_LED_PROGRAMMING;
	}
	// start command received
	else if ((((((uint16_t) dataReceived & 0x3f << 8) | (uint16_t)dataReceived) == secondaryStartCommand) || dataReceived & 0x3f == RC5_ADR_EXPERIMENTAL && dataReceived == (currentStopCommand + 1)) && (IDLE == currentState))
	{
		PORT_DRIVER_SET_STATE_ACTIVE();
		
		LED_DRIVER_POWER_ON_LED;
		
		EEPROM_WRITE_STATE(ACTIVE);
		
		currentState = ACTIVE;
	}
	// stop command received
	else if (dataReceived & 0x3f == RC5_ADR_EXPERIMENTAL && dataReceived == (currentStopCommand) && (ACTIVE == currentState))
	{
		PORT_DRIVER_SET_STATE_DEACTIVATED();
		
		LED_DRIVER_POWER_OFF_LED;
		
		EEPROM_WRITE_STATE(DEACTIVATED);
		
		LED_DRIVER_BLINK_LED_DEACTIVATED;
	}
}

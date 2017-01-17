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

// EEPROM addresses
#define EEPROM_ADDRESS_STATE_OF_MAIN_STATE_MACHINE		(unsigned char*) 0
#define EEPROM_ADDRESS_RECORDET_STOP_COMMAND			(unsigned char*) 1

// IO Pins
#define DO_LED_PIN			0	// OUTPUT pin used for LED output HIGH is ON LOW is OFF PWM is used over this IO PIN
#define DO_START_PIN		2	// OUTPUT pin used for control of user MCU. Indicates start with HIGH level and stop with LOW level
#define DO_KILL_SWITCH_PIN	1	// OUTPUT pin used for control of user kill switch. HIGH is INACTIVE LOW is ACTIVE

// States
#define POWERON			1
#define STARTED			2
#define STOPPED_SAFE	3
#define STOPPED			4

//////////////////////////////////////////////////////////////////////////
//					PROTOTYPES OF LOCAL FUNCTIONS
//////////////////////////////////////////////////////////////////////////
// EEPROM write function used for save writing to EEPROM
void EepromWriteByte(unsigned char* address, unsigned char data);

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
	// initialize the IR receiver sub module
	ir_init();
	
	// load the EEPROM data in RAM mirror
	unsigned char savedState = eeprom_read_byte(EEPROM_ADDRESS_STATE_OF_MAIN_STATE_MACHINE);
	unsigned char stopCommand = eeprom_read_byte(EEPROM_ADDRESS_RECORDET_STOP_COMMAND);
	
	// load preserved state of main state machine from EEPROM RAM mirror
	unsigned char  currentState = savedState;

	// set PORT B directions for DIO pins
	DDRB = (1 << DO_LED_PIN) | (1 << DO_KILL_SWITCH_PIN) | (1 << DO_START_PIN);
	
	// Set timer to 'Fast PWM' mode. Used for user LED
	TCCR0A |= (1 << WGM01) | (1 << WGM00);
	
	// set the PWM duty to ~4%
	OCR0A = 10;
	
	// shut down the LED PWM
	TCCR0A &= ~(1 << COM0A1);
	
	sei();

	while(1)
	{
		while(!has_next)
		{
			switch (currentState)
			{
				case POWERON:
				{
					// set the KILL pin to high
					PORTB = (1 << DO_KILL_SWITCH_PIN);
				}
				break;

				case STARTED:
				{
					// start the LED PWM on OUTPUT A
					TCCR0A |= (1 << COM0A1);
					
					// set start PIN and KILL PIN in case we are waking from power down and we are going to this mode
					PORTB |= (1 << DO_START_PIN) | (1 << DO_KILL_SWITCH_PIN);
				}
				break;

				case STOPPED_SAFE:
				{
					// shut down the LED PWM
					TCCR0A &= ~(1 << COM0A1);
					
					// stop the START and KILL signals to LOW
					PORTB = 0;
					
					EepromWriteByte(EEPROM_ADDRESS_STATE_OF_MAIN_STATE_MACHINE, STOPPED_SAFE);
					_delay_ms(1000);
					currentState = STOPPED;
					has_next = 0;
				}
				break;

				case STOPPED:
				{
					// shut down the LED PWM
					TCCR0A &= ~(1 << COM0A1);
					
					// write the state to EEPROM
					EepromWriteByte(EEPROM_ADDRESS_STATE_OF_MAIN_STATE_MACHINE, POWERON);

					// loop for ever, just for signalization
					for (;;)
					{
						// start the LED PWM on OUTPUT A
						TCCR0A |= (1 << COM0A1);
						
						// wait one second
						_delay_ms(1000);

						// shut down the LED PWM
						TCCR0A &= ~(1 << COM0A1);

						// wait one second
						_delay_ms(1000);
					}
				}
				break;
					// When eeprom is undefined we might end up here (after programming)
				default:
				{
					currentState = STOPPED;
				}
				break;
			}

			if (savedState != currentState && currentState != STOPPED)
			{
				EepromWriteByte(EEPROM_ADDRESS_STATE_OF_MAIN_STATE_MACHINE, currentState);
				_delay_ms(500);
				savedState = currentState;
			}
		}
		has_next = 0;

		if (addr == RC5_ADR_PROGRAMMING)
		{
			stopCommand = cmd & 0b11111110;
			
			// start the LED PWM on OUTPUT A
			TCCR0A |= (1 << COM0A1);

			_delay_ms(500);
			EepromWriteByte(EEPROM_ADDRESS_RECORDET_STOP_COMMAND, stopCommand);

			// shut down the LED PWM
			TCCR0A &= ~(1 << COM0A1);

			_delay_ms(500);
			
			// start the LED PWM on OUTPUT A
			TCCR0A |= (1 << COM0A1);

			_delay_ms(500);

			// shut down the LED PWM
			TCCR0A &= ~(1 << COM0A1);

			has_next = 0;
			currentState = POWERON;
		}
		else
		{
			switch (currentState)
			{
				case POWERON:
				{
					if (addr == RC5_ADR_EXPERIMENTAL)
					{
						if (cmd == (stopCommand + 1))
						{
							currentState = STARTED;
						}
						else if (cmd == stopCommand)
						{
							currentState = STOPPED_SAFE;
						}
					}
				}
				break;

				case STARTED:
				{
					if (addr == RC5_ADR_EXPERIMENTAL)
					{
						if (cmd == stopCommand)
						{
							currentState = STOPPED_SAFE;
						}
					}
				}
				break;
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//	NAME:	EepromWriteByte
//
//	DESCRIPTION:	Function is used for save writing to EEPROM.
//
//	ATRIBUTES:	
//				unsigned char* address
//					- INPUT the address in the memory where the data will be written
//					MUST BE a valid address of EEPROM memory,
//					otherwise memory corruption is possible
//
//				unsigned char data
//					- INPUT the data that is to be written to the address in EEPROM
//
//	RETURN VALUE:	-
//
//////////////////////////////////////////////////////////////////////////
void EepromWriteByte(unsigned char* address, unsigned char data)
{
	// stop interrupts as they may damage EEPROM
	cli();
	
	// wait for the EEPROM to be ready before writing
	eeprom_busy_wait();

	// write to EEPROM
	eeprom_write_byte(address, data);

	// return interrupts as normal
	sei();
}
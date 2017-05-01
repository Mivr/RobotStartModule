/*
 * eepromDriver.c
 *
 * Created: 3/25/2017 6:02:18 PM
 *  Author: MIVR
 */ 

#include "stdint.h"
#include "avr/eeprom.h"
#include "eepromDriver.h"
#include "avr/interrupt.h"

//////////////////////////////////////////////////////////////////////////
//	NAME:	EepromWriteByte
//
//	DESCRIPTION:	Function is used for safe writing to EEPROM.
//
//	ATRIBUTES:
//				uint8_t* address
//					- INPUT the address in the memory where the data will be written
//					MUST BE a valid address of EEPROM memory,
//					otherwise memory corruption is possible
//
//				uint8_t data
//					- INPUT the data that is to be written to the address in EEPROM
//
//	RETURN VALUE:	-
//
//////////////////////////////////////////////////////////////////////////
void EepromWriteByte(uint8_t* address, uint8_t data)
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
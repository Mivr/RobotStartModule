/*
 * eepromDriver.h
 *
 * Created: 3/25/2017 5:13:54 PM
 *  Author: MIVR
 */ 


#ifndef EEPROMDRIVER_H_
#define EEPROMDRIVER_H_

// EEPROM addresses
#define EEPROM_ADDRESS_STATE_OF_MAIN_STATE_MACHINE				(unsigned char*) 0
#define EEPROM_ADDRESS_RECORDED_STOP_COMMAND					(unsigned char*) 1
#define EEPROM_ADDRESS_RECORDED_SECONDARY_START_ADDRESS			(unsigned char*) 2
#define EEPROM_ADDRESS_RECORDED_SECONDARY_START_COMMAND			(unsigned char*) 3
#define EEPROM_ADDRESS_RECORDED_SECONDARY_STOP_ADDRESS			(unsigned char*) 4
#define EEPROM_ADDRESS_RECORDED_SECONDARY_STOP_COMMAND			(unsigned char*) 5

#define EEPROM_INITIALIZE(currentStateBuffer, stopCommandBuffer) \
*currentStateBuffer = eeprom_read_byte(EEPROM_ADDRESS_STATE_OF_MAIN_STATE_MACHINE);\
*stopCommandBuffer = eeprom_read_byte(EEPROM_ADDRESS_RECORDED_STOP_COMMAND)\


#define EEPROM_INITIALIZE_SECONDARY(secondaryStartData, secondaryStopData) \
*secondaryStartData = eeprom_read_byte(EEPROM_ADDRESS_RECORDED_SECONDARY_START_ADDRESS) << 8;\
*secondaryStartData |= eeprom_read_byte(EEPROM_ADDRESS_RECORDED_SECONDARY_START_COMMAND);\
*secondaryStopData = eeprom_read_byte(EEPROM_ADDRESS_RECORDED_SECONDARY_STOP_ADDRESS) << 8;\
*secondaryStopData |= eeprom_read_byte(EEPROM_ADDRESS_RECORDED_SECONDARY_STOP_COMMAND)\

#define  EEPROM_WRITE_STATE(stateToWriteToEEPROM)\
EepromWriteByte(EEPROM_ADDRESS_STATE_OF_MAIN_STATE_MACHINE, stateToWriteToEEPROM)

#define  EEPROM_WRITE_STOP_COMMAND(stopCommandToWriteToEEPROM)\
EepromWriteByte(EEPROM_ADDRESS_RECORDED_STOP_COMMAND, stopCommandToWriteToEEPROM)

// EEPROM write function used for save writing to EEPROM
void EepromWriteByte(uint8_t* address, uint8_t data);

#endif /* EEPROMDRIVER_H_ */
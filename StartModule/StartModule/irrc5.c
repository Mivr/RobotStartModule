/*
 * ir_rc5.c
 *
 * Created: 9/11/2016 9:57:44 PM
 *  Author: THE DEATH
 */ 

#define F_CPU	9600000UL

#include "ir.h"
#include "stdint.h"
#include "avr/io.h"
#include "avr/interrupt.h"


// Protocol definitions
#define ADDR_LEN	5
#define CMD_LEN		6

// Timings in us
#define HALF_BIT_LEN	889L
#define BIT_LEN			1778L

// Timer constants
#define MAX_ERROR		48	// ((F_CPU * 5) / 1000000)	// Maximum timing error in timer ticks
#define TIMER_PRESCALE	64

#define HALF_BIT_CNT	133 // ((HALF_BIT_LEN * (F_CPU / 1000000L)) / TIMER_PRESCALE)
#define BIT_CNT			266 // ((BIT_LEN * (F_CPU / 1000000L)) / TIMER_PRESCALE)

// Interrupt stuff
#define	IR_STARTBIT1	0
#define	IR_STARTBIT2	1
#define	IR_TOGGLEBIT	2
#define	IR_ADDR			3
#define	IR_CMD			4

// Variables
volatile uint16_t lastTimeTimerOverfllowed = 0;
uint8_t pulses[30], indexInPulsesBuffer = 0;

// Received data
uint16_t dataBuffer;

// magic decoding constants
#define SHORT_NEGATIVE_PULSE	0
#define SHORT_POSITIVE_PULSE	1
#define LONG_NEGATIVE_PULSE		2
#define LONG_POSITIVE_PULSE		3

void tryToDecode()
{
	uint8_t bitsCounter = 13;
	
	dataBuffer = 0;
	
	// start of stream detect
	if (pulses[0] > 9 && pulses[1] == SHORT_POSITIVE_PULSE)
	{
		uint8_t iterator = 2;
		while (iterator < 29)
		{
			if (pulses[iterator] < 4 && pulses[iterator + 1] < 4)
			{
				dataBuffer |= ((pulses[iterator] % 2 == 0 && pulses[iterator + 1] % 2 == 1) << bitsCounter);
				bitsCounter--;
				iterator += 2 - (pulses[iterator + 1] > 1);
			}
			else
			{
				iterator++;
			}
		}
	}
	
	if (indexInPulsesBuffer == 29)
	{
		for (uint8_t i = 0; i<29; i++)
		{
			pulses[i] = pulses[i+1];
		}
		
		indexInPulsesBuffer--;
	}
	
	handleEvent_dataReceivedOnIR(dataBuffer);
}

ISR (IR_PIN_ISR)
{
	uint16_t buffer = TCNT0 - lastTimeTimerOverfllowed - 65535 * ( TCNT0 < lastTimeTimerOverfllowed);
	pulses[indexInPulsesBuffer] = ( buffer > 1333 ) * 2 + !IR_GET_EDGE + 10 * (buffer < 444 || buffer > 2222);
	indexInPulsesBuffer++;
	lastTimeTimerOverfllowed = TCNT0;
	tryToDecode();
}





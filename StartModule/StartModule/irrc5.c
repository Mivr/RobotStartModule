/*
 * ir_rc5.c
 *
 * Created: 9/11/2016 9:57:44 PM
 *  Author: THE DEATH
 */ 

#define F_CPU	4800000UL

#include "ir.h"

#include <avr/io.h>
#include <avr/interrupt.h>


/*
 * Protocol definitions
 */
#define ADDR_LEN	5
#define CMD_LEN		6

/*
 * Timings in us
 */
#define HALF_BIT_LEN	889L
#define BIT_LEN			1778L

/*
 * Timer constants
 */
#define MAX_ERROR		((F_CPU * 5) / 1000000)	// Maximum timing error in timer ticks
#define TIMER_PRESCALE	64

#define HALF_BIT_CNT	((HALF_BIT_LEN * (F_CPU / 1000000L)) / TIMER_PRESCALE)	//111.12 @8MHz
#define BIT_CNT			((BIT_LEN * (F_CPU / 1000000L)) / TIMER_PRESCALE)		//222.25 @8MHz

/*
 * Interrupt stuff
 */
#define	IR_STARTBIT1	0
#define	IR_STARTBIT2	1
#define	IR_TOGGLEBIT	2
#define	IR_ADDR			3
#define	IR_CMD			4

/*
 * Variables
 */
volatile signed char status = IR_STARTBIT1;
volatile unsigned char has_next = 0;
volatile unsigned char timer_overflows;

/*
 * Received data
 */
volatile unsigned char cmd;
volatile unsigned char addr;

/*
 * Interrupt data
 */
register unsigned char curr_index asm("r2");


void ir_init(void) {
	/*
	 * Variables
	 */

	curr_index = 0;

	/*
	 * Init Timer0
	 */
	IR_CNT_PRESCALE_SET();
	IR_CNT = 0;
	IR_CNT_OVF_EN();

	/*
	 * Init INT0
	 */
	IR_INT_EN();		// Enable int0
	IR_ANY_EDGE_INT();	// Interrupt on any logical change
}

ISR (IR_PIN_ISR)
{
	static signed int cnt = 0;

	cnt += (signed int)IR_CNT + (signed int)(((unsigned int)timer_overflows) << 8);	// Save count
	IR_CNT = 0;
	timer_overflows = 0;

	switch (status)
	{
		case IR_STARTBIT1:
		{
			if (IR_EDGE_LOW())
			{
				status = IR_STARTBIT2;
				cnt = 0;
			}
		}
		break;
		
		default:
		{
			if ((cnt - MAX_ERROR) < BIT_CNT && (cnt + MAX_ERROR) > BIT_CNT)
			{
				switch(status)
				{
					case IR_STARTBIT2:
					{
						status = IR_TOGGLEBIT;
						cnt = 0;
					}
					break;
					case IR_TOGGLEBIT:
					{
						curr_index = ADDR_LEN;
						addr = 0;
						cmd = 0;
						cnt = 0;
						status = IR_ADDR;
					}
					break;
					case IR_ADDR:
					{
						curr_index--;
						if (IR_EDGE_LOW())
						{
							addr |= _BV(curr_index);
						}

						if (!curr_index)
						{
							curr_index = CMD_LEN;
							status = IR_CMD;
						}
						cnt = 0;
					}
					break;
					case IR_CMD:
					{
						curr_index--;
						if (IR_EDGE_LOW())
						{
							cmd |= _BV(curr_index);
						}

						if (!curr_index)
						{
							has_next = 1;
							status = IR_STARTBIT1;
						}
						cnt = 0;
					}
					break;
				}
			}
			else if ((cnt - MAX_ERROR) < HALF_BIT_CNT && (cnt + MAX_ERROR) > HALF_BIT_CNT)
			{
				cnt = HALF_BIT_CNT;	// Synchronize..
			}
			else
			{
				status = IR_STARTBIT1;
			}
		}
		break;
	}
}

ISR (IR_CNT_ISR)
{
	timer_overflows++;
	if (timer_overflows > 50)
	{
		status = IR_STARTBIT1;
	}
}




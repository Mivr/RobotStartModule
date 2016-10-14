/*
 * ir.h
 *
 * Created: 9/11/2016 9:58:16 PM
 *  Author: THE DEATH
 */ 


#ifndef IR_H_
#define IR_H_

/*
 * HW-dependent stuff. Change these macros if your hardware differs.
 */

/*
 * Start module (Attiny13A)
 */
#define IR_INT_EN() GIMSK |= (1 << PCIE)
#define IR_ANY_EDGE_INT()	PCMSK |= (1 << PCINT4);

#define IR_CNT_PRESCALE_SET()	TCCR0B |= (1 << CS00) | (1 << CS01)	// Prescaler 64
#define IR_CNT TCNT0
#define IR_CNT_OVF_EN()	TIMSK0 |= (1 << TOIE0)

#define IR_PIN_ISR PCINT0_vect
#define IR_CNT_ISR TIM0_OVF_vect

#define IR_GET_EDGE()	(PINB & (1 << 4))
#define IR_EDGE_LOW()	!IR_GET_EDGE()
#define IR_EDGE_HIGH()	IR_GET_EDGE()
// ------------- END HW-stuff -------------------- //

/*
 * IR Struct
 */
typedef struct {
	unsigned char address_low;
	unsigned char command;
} IRDATA;

/*
 * Functions
 */
void ir_init(void);

/*
 * Shared variables
 */
extern volatile unsigned char has_next;
extern volatile unsigned char addr;
extern volatile unsigned char cmd;

/*
 * RC5 protocol standard addresses and commands
 */
#define RC5_ADR_TV1				0x00
#define RC5_ADR_TV2				0x01
#define RC5_ADR_TELETEXT		0x02
#define RC5_ADR_VIDEO			0x03
#define RC5_ADR_LV1				0x04
#define RC5_ADR_VCR1			0x05
#define RC5_ADR_VCR2			0x06
#define RC5_ADR_EXPERIMENTAL	0x07
#define RC5_ADR_SAT1			0x08
#define RC5_ADR_CAMERA			0x09
#define RC5_ADR_SAT2			0x0A
#define RC5_ADR_PROGRAMMING		0x0B
#define RC5_ADR_CDV				0x0C
#define RC5_ADR_CAMCORDER		0x0D
#define RC5_ADR_MODESWITCH		0x0F
#define RC5_ADR_PREAMP			0x10
#define RC5_ADR_TUNER			0x11
#define RC5_ADR_RECORDER1		0x12
#define RC5_ADR_PREAMP2			0x13
#define RC5_ADR_CDPLAYER		0x14
#define RC5_ADR_PHONO			0x15
#define RC5_ADR_SATA			0x16
#define RC5_ADR_RECORDER2		0x17
#define RC5_ADR_CDR				0x1A
#define RC5_ADR_LIGHTING		0x1D
#define RC5_ADR_LIGHTING2		0x1E
#define RC5_ADR_PHONE			0x1F

#define RC5_CMD_0				0x00
#define RC5_CMD_1				0x01
#define RC5_CMD_2				0x02
#define RC5_CMD_3				0x03
#define RC5_CMD_4				0x04
#define RC5_CMD_5				0x05
#define RC5_CMD_6				0x06
#define RC5_CMD_7				0x07
#define RC5_CMD_8				0x08
#define RC5_CMD_9				0x09
#define RC5_CMD_MIN				0x0A
#define RC5_CMD_STANDBY			0x0C
#define RC5_CMD_MUTE			0x0D
#define RC5_CMD_VPLUS			0x10
#define RC5_CMD_VMIN			0x11
#define RC5_CMD_BPLUS			0x12
#define RC5_CMD_BMIN			0x13
#define RC5_CMD_PPLUS			0x20
#define RC5_CMD_PMIN			0x21
#define RC5_CMD_FRWD			0x32
#define RC5_CMD_FFWD			0x34
#define RC5_CMD_PLAY			0x35
#define RC5_CMD_STOP			0x36
#define RC5_CMD_RECORDING		0x37

#endif /* IR_H_ */
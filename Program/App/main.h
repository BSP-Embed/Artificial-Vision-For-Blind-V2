#ifndef MAIN_H
#define MAIN_H

#include "includes.h"

#ifdef TESTING
	#define PhNum1		"9980237552"
#else
	#define PhNum1		"8971053656"
#endif

#define SOIL_SENS_DDR			DDRD
#define SOIL_SENS_PPIN			PIND
#define SOIL_SENS_PORT			PORTD
#define SOIL_SENS_PIN			PD4

#define MSG_WAIT_MSG			1
#define MSG_PH_NUM				2
#define MSG_COLL_MSG			3
#define MSG_RCV_MSG				4

#define LINE_FEED				0x0A
#define DIST_THRES				0.5
#define ULTRASONIC_CONST		0.01724

#define APR_DDR			DDRC
#define APR_PORT		PORTC

struct  {
	volatile int8u msg:1;
	volatile int8u Meas:1;
	volatile int8u Sw:1;
	volatile int8u PIR:1;

}Flag;

//DEFINE MACROS
#define StartTmr()					TCCR0  	|= _BV(CS01)
#define StopTmr()					TCCR0  	&= ~_BV(CS01)

#define EnUARTInt()					UCSRB |= _BV(RXCIE); UCSRA |= _BV(RXC)
#define DisUARTInt()				UCSRB &= ~_BV(RXCIE); UCSRA &= ~_BV(RXC)

//FUNCTION PROTOTYPES
static	void	init		(void);
static	void	dispMsg		(void);
static	void 	tmr1init	(void);
static	int8u	checkmsg	(void);
static	void	Flagsinit	(void);
static	void	EXTinit		(void);
static	void	EnVoice		(int8u ch);
		void	MeasDist	(int8u sens,int8u disp, float *dist);
static	void	tmr0init	(void);
static	void	calcdist	(int8u sens);
		void	ultinit		(void);
static	void	DispTitle	(void);
static void		GPIOInit	(void);
#endif

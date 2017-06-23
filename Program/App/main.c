#include "main.h"

const char *MSG[] = {	"Your Person is ", 
						"I'm in danger &  " };
extern int8u lcdptr;
					
int8u phnum[15];
int8u sbuf[160];
static volatile int16u ultpulse;

int main(void)
{
	int8u i;
	float dist;
	
	init();
	
	while (TRUE) {
		
		if (Flag.msg) {
			DisUARTInt();
			Flag.msg = 0;
			if (checkmsg()) {
				beep(1,250);
				SendLinkLoc(PhNum1, MSG[0]);
				DispTitle();
			} else 
				beep(1,750);
			EnUARTInt();
		}
		
		if (Flag.Sw) {
			DisUARTInt();
			Flag.Sw = FALSE;
			beep(1,150);	
			SendLinkLoc(PhNum1, MSG[1]);
			DispTitle();
			EnUARTInt();
		}
				
		if (Flag.PIR) {
			Flag.PIR = FALSE;
			beep(1,150);
			EnVoice(4);
		}
		
		if (Flag.Meas) {
			Flag.Meas = FALSE;
			for (i = 1; i <= 3; i++) {
				MeasDist(i, 0, &dist);
				if (dist < DIST_THRES) {
					beep(1,150);
					EnVoice(i);
				}
			}
			
			if ((SOIL_SENS_PPIN & _BV(SOIL_SENS_PIN)) == 0) {
				beep(1,150);
				EnVoice(5);
			}
			
		}
		
		
		sleep();
	}
	return 0;
}
static void init(void)
{
	buzinit();
	ledinit();
	beep(2,100);
	GPIOInit();
	lcdinit();
	uartinit();
	tmr1init();
	EXTinit();
	ultinit();
	GPSInit();
	GSMinit();
	Flagsinit();
	sei();
	DispTitle();
	EnUARTInt();
}
static void EnVoice(int8u ch)
{
	APR_PORT &= ~_BV(ch+2);
	dlyms(100);
	APR_PORT |= _BV(ch+2);
	dlyms(2500);
}
static void Flagsinit(void)
{
	Flag.msg = FALSE;
	Flag.Meas = FALSE;
	Flag.Sw = FALSE;
	Flag.PIR = FALSE;
}

static void tmr1init(void)
{
	TCNT1H   = 0xD3;
	TCNT1L   = 0x00;
	TIMSK   |= _BV(TOIE1);			//ENABLE OVERFLOW INTERRUPT
	TCCR1A   = 0x00;					
	TCCR1B  |= _BV(CS10) | _BV(CS11); /* PRESCALAR BY 16 */
}

static void EXTinit(void)
{
	INTDDR 	&= ~_BV(INT0_PIN);
	INTPORT |= _BV(INT0_PIN);

	INTDDR 	&= ~_BV(INT1_PIN);
	INTPORT |= _BV(INT1_PIN);

	GICR |= _BV(INT0) | _BV(INT1);			//ENABLE EXTERNAL INTERRUPT
	MCUCR |= _BV(ISC01) | _BV(ISC11);		//FALLING EDGE INTERRUPT

}
ISR(INT0_vect)
{
	Flag.PIR = TRUE;
	GICR |= _BV(INT0);
}
ISR(INT1_vect)
{
	Flag.Sw = TRUE;
	GICR |= _BV(INT1);
}
/* overflows at every 100msec */
ISR(TIMER1_OVF_vect) 
{ 
	static int8u i, j;

	TCNT1H = 0xD3;
	TCNT1L = 0x00;
	
	if (++i >= 50) 
		 i = 0;
			
	switch(i) {
		case 0: case 2: ledon(); break;
		case 1: case 3: ledoff(); break;
	} 
	
	if (++j >= 5) {
		Flag.Meas = TRUE;
		j = 0;
	}
	
}

ISR (USART_RXC_vect)
{
	static int8u i;
	static int8u msgcnt,phcnt;
	static int8u state = MSG_WAIT_MSG;

	switch (state) {
		case MSG_WAIT_MSG:
		if ( UDR == '\"') state = MSG_PH_NUM;
		break;
		case MSG_PH_NUM:
		if (phcnt++ < 13)
		phnum[phcnt-1] = UDR;
		else
		state = MSG_COLL_MSG;
		break;
		case MSG_COLL_MSG:
		if (UDR == LINE_FEED)
		state = MSG_RCV_MSG;
		break;
		case MSG_RCV_MSG:
		if ((sbuf[msgcnt++] = UDR) == LINE_FEED) {
			sbuf[msgcnt-2] = '\0';
			for (i = 0 ; i < 10; i++)	/* eliminate +91 */
			phnum[i] = phnum[i+3];
			phnum[i] = '\0';
			state = MSG_WAIT_MSG;
			msgcnt = 0;
			phcnt = 0;
			Flag.msg = 1;
			DisUARTInt();
		}
	}
}
static int8u checkmsg(void)
{
	if (!strcmp(sbuf, "track"))
		return 1;
	else
		return 0;
}

void MeasDist(int8u sens,int8u disp, float *dist)
{
	
	int8u i;
	int32u cnt;
	char s[10];
	
	cnt = 0;
	
	for (i = 0; i < 8; i++) {
		calcdist(sens);
		cnt += ultpulse;
	}
	cnt >>= 3;

	*dist = (cnt * ULTRASONIC_CONST);

	if (disp != 0) {
		lcdptr = disp;
		lcdws("    ");
		ftoa(*dist,s,2);
		lcdptr = disp;
		lcdws(s);
	}
}
static void calcdist(int8u sens)
{
	switch (sens) {
		case 1:
				ULTSEN1_PORT		|= _BV(TRIG1_PIN);
				dlyus(10);
				ULTSEN1_PORT		&= ~_BV(TRIG1_PIN);
				
				ultpulse = 0;

				while ((ULTSEN1_PIN & _BV(ECHO1_PIN)) == 0);
				StartTmr();
				while (ULTSEN1_PIN & _BV(ECHO1_PIN));
				StopTmr();
				break;
		case 2:
				ULTSEN2_PORT		|= _BV(TRIG2_PIN);
				dlyus(10);
				ULTSEN2_PORT		&= ~_BV(TRIG2_PIN);
				
				ultpulse = 0;

				while ((ULTSEN2_PIN & _BV(ECHO2_PIN)) == 0);
				StartTmr();
				while (ULTSEN2_PIN & _BV(ECHO2_PIN));
				StopTmr();
				break;
		case 3:
				ULTSEN3_PORT		|= _BV(TRIG3_PIN);
				dlyus(10);
				ULTSEN3_PORT		&= ~_BV(TRIG3_PIN);
				
				ultpulse = 0;

				while ((ULTSEN3_PIN & _BV(ECHO3_PIN)) == 0);
				StartTmr();
				while (ULTSEN3_PIN & _BV(ECHO3_PIN));
				StopTmr();
				break;
	}
	
}
void ultinit(void)
{
	ULTSEN1_DDR 		|= _BV(TRIG1_PIN);
	ULTSEN1_DDR 		&= ~_BV(ECHO1_PIN);

	ULTSEN1_PORT		&= ~_BV(TRIG1_PIN);
	ULTSEN1_PORT		|= _BV(ECHO1_PIN);
	
	ULTSEN2_DDR 		|= _BV(TRIG2_PIN);
	ULTSEN2_DDR 		&= ~_BV(ECHO2_PIN);

	ULTSEN2_PORT		&= ~_BV(TRIG2_PIN);
	ULTSEN2_PORT		|= _BV(ECHO2_PIN);
	
	ULTSEN3_DDR 		|= _BV(TRIG3_PIN);
	ULTSEN3_DDR 		&= ~_BV(ECHO3_PIN);

	ULTSEN3_PORT		&= ~_BV(TRIG3_PIN);
	ULTSEN3_PORT		|= _BV(ECHO3_PIN);
	
	tmr0init();
}
static void GPIOInit(void)
{
	SOIL_SENS_DDR &= ~_BV(SOIL_SENS_PIN);
	SOIL_SENS_PORT |= _BV(SOIL_SENS_PIN);
	
	APR_DDR	= 0xFF;
	APR_PORT |= 0xF8;
	
}
static void tmr0init(void)
{
	TCNT0   =  167;
	TIMSK   |= _BV(TOIE0);			//ENABLE OVERFLOW INTERRUPT
	
}
/* OverFlows every 100us */
ISR(TIMER0_OVF_vect)
{
	TCNT0 = 167;
	++ultpulse;
}
static void DispTitle(void)
{
	lcdclr();
	lcdws("ArtificialVision");
}
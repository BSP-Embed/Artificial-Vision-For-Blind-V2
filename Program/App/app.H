#ifndef _APP_H
#define _APP_H

/* Main settings */

/* comment the below line for release */
#define TESTING
//#define DEMO

#define AL_SENSOR_PIN			PIND
#define AL_SENSOR				PD2

/*********** EXTERNAL INTERRUPT PINS ***************/
#define INTDDR					DDRD
#define INTPORT					PORTD
#define INTPIN					PIND
#define INT0_PIN				PD2
#define INT1_PIN				PD3

#define EEPROM_DISPLAY			0

/******** MAIN CONFIGURATION ENDS ****************/

/* LCD CONFIGURATION */
#define LCD_DDR           	   		DDRB 
#define LCD_DATA_PORT          		PORTB
#define LCD_ENABLE_PIN        		PB3 
#define LCD_REGISTER_SELECT_PIN     PB2
#ifdef TESTING
	#define DISPLAY_INIT				0
#else
	#define DISPLAY_INIT				1
#endif
#define LCD_NSCRL					1
#define FLOAT_T0_STRING				1
#define DECIMAL_PRECISION			1000000

/******** LCD CONFIGURATION ENDS ****************/

/* MOTOR CONFIGURATION */

#define MOT_DDR				DDRC
#define MOT_PORT			PORTC
#define MOT_ANODE			PC2
#define MOT_CATHODE			PC3
/******** LCD CONFIGURATION ENDS ****************/


/* BUZZER: CONFIGURATION */
#define	BUZ_DDR						DDRC
#define	BUZ_PORT					PORTC
#define	BUZ_PIN						PC1
#define OFF_TIME					100
/******** BUZZER CONFIGURATION ENDS ****************/

/* LED: CONFIGURATION */

#define	LED_DDR			DDRC
#define	LED_PORT		PORTC
#define	LED_PIN			PC0

#define OFF_TIME		100
/******** LED CONFIGURATION ENDS ****************/

/* GSM: CONFIGURATION */

#define MSG_START					""
#define MSG_END						". Automated SMS By:AV."

/******** ADC CONFIGURATION ENDS ****************/
#define REF_VOL_256

/******** GSM CONFIGURATION BEGINS ****************/
#define GSM_GPS_DDR				DDRC
#define GSM_GPS_PORT			PORTC
#define GSM_GPS_PIN				PC2
				
#define GSMEn()				do {									\
								GSM_GPS_PORT &= ~_BV(GSM_GPS_PIN); 	\
								dlyms(500);							\
							} while(0)

#define GPSEn()				do {									\
								GSM_GPS_PORT |= _BV(GSM_GPS_PIN);	\
								dlyms(500);							\
							} while(0)
/******** GSM CONFIGURATION ENDS ****************/

/******** EEPROM CONFIGURATION ****************/
#define EEPROM_DISPLAY		0


/******* ULTRASNOIC SENSOR CONFIGURATION    *******/
#define	ULTSEN1_DDR			DDRB
#define	ULTSEN1_PORT		PORTB
#define	ULTSEN1_PIN			PINB
#define ECHO1_PIN			PB1
#define TRIG1_PIN			PB0

#define	ULTSEN2_DDR			DDRA
#define	ULTSEN2_PORT		PORTA
#define	ULTSEN2_PIN			PINA
#define ECHO2_PIN			PA1
#define TRIG2_PIN			PA0

#define	ULTSEN3_DDR			DDRD
#define	ULTSEN3_PORT		PORTD
#define	ULTSEN3_PIN			PIND
#define ECHO3_PIN			PD6
#define TRIG3_PIN			PD7
/******* ULTRASNOIC SENSOR CONFIGURATION  ENDS *******/

#endif

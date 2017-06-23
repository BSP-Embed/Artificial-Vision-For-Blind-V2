#ifndef GPS_H
#define GPS_H

//INCLUDE HEADER FILES
#include "includes.h"

/*********** TESTING or RELEASE SETTINGS ***********/
/*comment below line for release*/

#define TESTING

#ifdef TESTING
	#define PhNum	 "9980237552"
#else
	#define PhNum	 "9900908675"
#endif

#define GSMEn()				do {									\
								GSM_GPS_PORT &= ~_BV(GSM_GPS_PIN); 	\
								dlyms(500);							\
							} while(0)

#define GPSEn()				do {									\
								GSM_GPS_PORT |= _BV(GSM_GPS_PIN);	\
								dlyms(500);							\
							} while(0)

//FUNCTION PROTOTYPE
void GPSInit	(void);
void GPSgetloc	(int8u *Lat, int8u *Lon);

#endif
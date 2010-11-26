#ifndef CGPS_H
#define CGPS_H

// include the NMEA files that are included in this class
#include ".\Resources\gps\cnmea.h"
#include ".\Resources\serial\CUart.h"

#define NMEA_BAUD_4800		"4800,"
#define NMEA_BAUD_9600		"9600,"
#define NMEA_BAUD_19200		"19200,"
#define NMEA_BAUD_38400		"38400,"
#define NMEA_BAUD_57600		"57600,"
#define NMEA_BAUD_115200	"115200,"

#define USE_GGA		0x01
#define USE_GGA5	0x02
#define USE_GSA		0x04
#define USE_GSV		0x08
#define USE_VTG		0x10
#define USE_GLL		0x20
#define USE_RMC		0x40
#define USE_RF		0x80
#define USE_MAX		0xff

#define BROADCAST_ALWAYS	0x00
#define BROADCASE_ONFIX		0x01

#define READ		0x20
#define WRITE		0x10
#define MAX_ID		16
#define MAX_BUF		256

class CGps	: 	public CNmea
{
private:
	CUart3 _data;
	uint8_t tempPacket[256];
	uint8_t wptr;
									
	// This will format the outcoming messages
	void confignmeasentences(char*, uint8_t, uint8_t);
	void sendwarmstart();
	void sendcoldstart();
	void sendhotstart();

public:
	CGps(PinName, PinName);			/* constructor */
	CGps();
	
	void Enable();
	void Disable();
	void Restart();
	void ChangeMaskAngle(char*);	/* calibrate receiver for rough terain */
	void RequestPostitionError();	/* ask the host for some info */
	void BuildPacket();
};

#endif


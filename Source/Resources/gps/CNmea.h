#ifndef CNMEA_H
#define CNMEA_H

// peripheral library access
#include ".\mbedLibrary\mbed.h"

#ifndef TRUE
#define TRUE	0x1
#define FALSE	0x0
#endif

typedef struct
{
	char* lat;
	char* lat_dir;
	char* lon;
	char* lon_dir;
	char* qual;
	char* num_sat;
}GGA_T;

typedef struct
{
	char* speed_kmh;
	char* course_true;
	char* course_magnetic;
}VTG_T;

typedef struct
{
	char* lat;
	char* lat_dir;
	char* lon;
	char* lon_dir;
	char* heading;
	char* pos_stat;
}RMC_T;

typedef struct
{
	char* lat;
	char* lat_dir;
	char* lon;
	char* lon_dir;
	char* status;	
}GLL_T;

class CNmea
{
private:	
	// NMEA packet types for the loading of structures
	uint8_t ParseGGA(char*);
	uint8_t ParseVTG(char*);
	uint8_t ParseRMC(char*);
	uint8_t ParseGLL(char*);
	uint8_t tempNMEAsentence[256];

protected:
	// objects for parsing data
	GGA_T tempgga;
	VTG_T tempvtg;
	RMC_T temprmc;
	GLL_T tempgll;

public:
	// user access
	CNmea();

	// not sure how data will be called here but this is where it belongs
	// open parameters to take the packet
	void ParseNmeaPacket(uint8_t*);
	uint8_t* GetNmeaPacket();


};

#endif

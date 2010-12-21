#ifndef CNMEA_H
#define CNMEA_H

// peripheral library access
#include ".\mbedLibrary\mbed.h"

typedef struct GGA_T GGA_T;
struct GGA_T{
	char* sentence;
	char* lat;
	char* lat_dir;
	char* lon;
	char* lon_dir;
	char* qual;
	char* num_sat;
};

typedef struct VTG_T VTG_T;
struct VTG_T{
	char* sentence;
	char* speed_kmh;
	char* course_true;
	char* course_magnetic;
};

typedef struct RMC_T RMC_T;
struct RMC_T{
	char* sentence;
	char* lat;
	char* lat_dir;
	char* lon;
	char* lon_dir;
	char* heading;
	char* pos_stat;
};

typedef struct GLL_T GLL_T;
struct GLL_T{
	char* sentence;
	char* lat;
	char* lat_dir;
	char* lon;
	char* lon_dir;
	char* status;	
};

class CNmea
{
private:	
	// NMEA packet types for the loading of structures
	uint32_t ParseGGA(char*);
	uint32_t ParseVTG(char*);
	uint32_t ParseRMC(char*);
	uint32_t ParseGLL(char*);
	char tempNMEAsentence[256];

protected:
	// objects for parsing data
	GGA_T tempgga;
	VTG_T tempvtg;
	RMC_T temprmc;
	GLL_T tempgll;

public:
	// objects for parsing data
	GGA_T gga;
	VTG_T vtg;
	RMC_T rmc;
	GLL_T gll;


public:
	// user access
	CNmea();

	// not sure how data will be called here but this is where it belongs
	// open parameters to take the packet
	void ParseNmeaPacket(char*);
	char* GetNmeaPacket();


};

#endif

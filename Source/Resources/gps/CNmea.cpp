/*
 *  CSULB
 *  02-11-10
 *  sgrove
 *
 *    NOTES:
 *        	This class will be responsible for all nmea packet parsing
 *				and information that is ripped from the sattelites.
 *				User will make calls here to get lon, lat, heading ect...
 *
 *			Part of the project and compiling.  Needs to have each header 
 *				broken down into a method of its own and then parse the 
 *				methods.  This will keep the code more modular and easier
 *				to optimize later -> if that time ever comes.
 */

#include "main.h"
#include "CNmea.h"


CNmea::CNmea()
{
	// should initialize all structure buffers to 0 
	// but the compilier may do that for us.
	gga.sentence = (char*)malloc(128 * sizeof(char));
	gga.lat = (char*)malloc(11 * sizeof(char));
	gga.lat_dir = (char*)malloc(2 * sizeof(char));
	gga.lon = (char*)malloc(11 * sizeof(char));
	gga.lon_dir = (char*)malloc(2 * sizeof(char));
	gga.qual = (char*)malloc(2 * sizeof(char));
	gga.num_sat = (char*)malloc(3 * sizeof(char));
	
	vtg.sentence = (char*)malloc(128 * sizeof(char));
	vtg.speed_kmh = (char*)malloc(16 * sizeof(char));
	vtg.course_true = (char*)malloc(16 * sizeof(char));
	vtg.course_magnetic = (char*)malloc(16 * sizeof(char));

	rmc.sentence = (char*)malloc(128 * sizeof(char));
	rmc.lat = (char*)malloc(11 * sizeof(char));
	rmc.lat_dir = (char*)malloc(2 * sizeof(char));
	rmc.lon = (char*)malloc(11 * sizeof(char));
	rmc.lon_dir = (char*)malloc(2 * sizeof(char));
	rmc.heading = (char*)malloc(3 * sizeof(char));
	rmc.pos_stat = (char*)malloc(2 * sizeof(char));
	
	gll.sentence = (char*)malloc(128 * sizeof(char));
	gll.lat = (char*)malloc(11 * sizeof(char));
	gll.lat_dir = (char*)malloc(2 * sizeof(char));
	gll.lon = (char*)malloc(11 * sizeof(char));
	gll.lon_dir = (char*)malloc(2 * sizeof(char));
	gll.status = (char*)malloc(2 * sizeof(char));
}

uint32_t CNmea::ParseGGA(char* input)
{
	if (memcmp(input, "GPGGA", 5) == NULL){
		memset(gga.sentence, 0x0, strlen(gga.sentence));
		memcpy(gga.sentence, input, strlen(input));
		strtok(input, ",");						/* parse the header */
		strtok(NULL, ",");						/* parse the UTC */
		gga.lat = strtok(NULL, ",");			/* parse the latitude */
		gga.lat_dir = strtok(NULL, ",");		/* parse the latitude direction */
		gga.lon = strtok(NULL, ",");			/* parse the longitude */
		gga.lon_dir = strtok(NULL, ",");		/* parse the longitude direction */
		gga.qual = strtok(NULL, ",");			/* parse the integrity of the signal */
		gga.num_sat = strtok(NULL, ",");		/* parse the amount of sats used for the locaiton */
		//strtok(NULL, ",");					/* parse the horizontal dilution of precision */
		//strtok(NULL, ",");					/* parse the antenna altitude */
		//strtok(NULL, ",");					/* parse the unit of measure */
		//strtok(NULL, ",");					/* parse the geoidal seperation */		
		//strtok(NULL, ",");					/* parse the unit of seperation */
		//strtok(NULL, ",");					/* parse **unused */		
		//strtok(NULL, ",");					/* parse **unused */
		//memset(tempgga.sentence, 0x0, strlen(tempgga.sentence));
		//memcpy(tempgga.sentence, input, strlen(input));
		return (1);
	}
	return (0);
}

uint32_t CNmea::ParseRMC(char* input)
{
 	if (memcmp(input, "GPRMC", 5) == NULL){
		memset(rmc.sentence, 0x0, strlen(rmc.sentence));
		memcpy(rmc.sentence, input, strlen(input));
		strtok(input, ",");							/* parse the header */
		rmc.pos_stat = strtok(NULL, ",");			/* parse the UTC */
		rmc.lat = strtok(NULL, ",");				/* parse the position status */
		rmc.lat_dir = strtok(NULL, ",");			/* parse the latitude */
		rmc.lon = strtok(NULL, ",");				/* parse the latitude direction */
		rmc.lon_dir = strtok(NULL, ",");			/* parse the longitude */
		strtok(NULL, ",");							/* parse the longitude direction */
		rmc.heading = strtok(NULL, ",");			/* parse the speed in knots */
		//strtok(NULL, ",");						/* parse the heading in degree (true) */
		//strtok(NULL, ",");						/* parse the date */
		//strtok(NULL, ",");						/* parse the magnetic variation in degrees (future)  */
		//strtok(NULL, ",");						/* parse the magnetic variation (future)*/
		//memset(temprmc.sentence, 0x0, strlen(temprmc.sentence));
		//memcpy(temprmc.sentence, input, strlen(input));
		return (1);
	}
	return (0);
}

uint32_t CNmea::ParseVTG(char* input)
{
	if (memcmp(input, "GPVTG", 5) == NULL){
		memset(vtg.sentence, 0x0, strlen(vtg.sentence));
		memcpy(vtg.sentence, input, strlen(input));
		strtok(input, ",");							/* parse the header */
		vtg.course_true = strtok(NULL, ",");		/* parse the course */
		strtok(NULL, ",");							/* parse the identifier */
		vtg.course_magnetic = strtok(NULL, ",");	/* parse the course (magnetic) */
		strtok(NULL, ",");							/* parse the identifier */
		strtok(NULL, ",");							/* parse the speed in knots*/
		strtok(NULL, ",");							/* parse the identifier */
		vtg.speed_kmh = strtok(NULL, ",");			/* parse the speed in kilometers */
		//strtok(NULL, ",");						/* parse the identifier */
		//memset(tempvtg.sentence, 0x0, strlen(tempvtg.sentence));
		//memcpy(tempvtg.sentence, input, strlen(input));
		return (1);
	}
	return (0);
}

uint32_t CNmea::ParseGLL(char* input)
{
	if (memcmp(input, "GPGLL", 5) == NULL){
		memset(gll.sentence, 0x0, strlen(gll.sentence));
		memcpy(gll.sentence, input, strlen(input));
		strtok(input, ",");							/* parse the header */
		gll.lat = strtok(NULL, ",");				/* parse the latitude */
		gll.lat_dir = strtok(NULL, ",");			/* parse the latitude direction */
		gll.lon = strtok(NULL, ",");				/* parse the longitude */
		gll.lon_dir = strtok(NULL, ",");			/* parse the longitude direction */
		strtok(NULL, ",");							/* parse the UTC */
		gll.status = strtok(NULL, ",");				/* parse the status */
		//memset(tempgll.sentence, 0x0, strlen(tempgll.sentence));
		//memcpy(tempgll.sentence, input, strlen(input));
		return (1);
	}
	return (0);
}

void CNmea::ParseNmeaPacket(char* input)
{
	// delete old contents and store whats new
	memset(tempNMEAsentence, 0x00, strlen(tempNMEAsentence));
	memcpy(tempNMEAsentence, input, strlen(input+1));
	// look for a sentence match and parse if found
	// store in a global accessable objet
	if (ParseGGA(input) == 1){
		return;
	}
	else if (ParseRMC(input) == 1){
		return;
	}
	else if (ParseVTG(input) == 1){
		return;
	}
	else if (ParseGLL(input) == 1){
		return;
	}
}

char* CNmea::GetNmeaPacket()
{
	return tempNMEAsentence;
}

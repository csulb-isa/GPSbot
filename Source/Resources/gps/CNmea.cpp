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

#include "CNmea.h"

CNmea::CNmea()
{
	// should initialize all structure buffers to 0 
	// but the compilier may do that for us.
}

uint32_t CNmea::ParseGGA(char* input)
{
	if (memcmp(input, "GPGGA", 5) == NULL)
	{
		strtok(input, ",");						/* parse the header */
		strtok(NULL, ",");						/* parse the UTC */
		tempgga.lat = strtok(NULL, ",");		/* parse the latitude */
		tempgga.lat_dir = strtok(NULL, ",");	/* parse the latitude direction */
		tempgga.lon = strtok(NULL, ",");		/* parse the longitude */
		tempgga.lon_dir = strtok(NULL, ",");	/* parse the longitude direction */
		tempgga.qual = strtok(NULL, ",");		/* parse the integrity of the signal */
		tempgga.num_sat = strtok(NULL, ",");	/* parse the amount of sats used for the locaiton */
		//strtok(NULL, ",");					/* parse the horizontal dilution of precision */
		//strtok(NULL, ",");					/* parse the antenna altitude */
		//strtok(NULL, ",");					/* parse the unit of measure */
		//strtok(NULL, ",");					/* parse the geoidal seperation */		
		//strtok(NULL, ",");					/* parse the unit of seperation */
		//strtok(NULL, ",");					/* parse **unused */		
		//strtok(NULL, ",");					/* parse **unused */
		memset(tempgga.sentence, 0x0, strlen(tempgga.sentence));
		memcpy(tempgga.sentence, input, strlen(input));
		return (1);
	}
	return (0);
}

uint32_t CNmea::ParseRMC(char* input)
{
 	if (memcmp(input, "GPRMC", 5) == NULL)
	{
		strtok(input, ",");							/* parse the header */
		temprmc.pos_stat = strtok(NULL, ",");		/* parse the UTC */
		temprmc.lat = strtok(NULL, ",");			/* parse the position status */
		temprmc.lat_dir = strtok(NULL, ",");		/* parse the latitude */
		temprmc.lon = strtok(NULL, ",");			/* parse the latitude direction */
		temprmc.lon_dir = strtok(NULL, ",");		/* parse the longitude */
		strtok(NULL, ",");							/* parse the longitude direction */
		temprmc.heading = strtok(NULL, ",");		/* parse the speed in knots */
		//strtok(NULL, ",");						/* parse the heading in degree (true) */
		//strtok(NULL, ",");						/* parse the date */
		//strtok(NULL, ",");						/* parse the magnetic variation in degrees (future)  */
		//strtok(NULL, ",");						/* parse the magnetic variation (future)*/
		memset(temprmc.sentence, 0x0, strlen(temprmc.sentence));
		memcpy(temprmc.sentence, input, strlen(input));
		return (1);
	}
	return (0);
}

uint32_t CNmea::ParseVTG(char* input)
{
	if (memcmp(input, "GPVTG", 5) == NULL)
	{
		strtok(input, ",");							/* parse the header */
		tempvtg.course_true = strtok(NULL, ",");	/* parse the course */
		strtok(NULL, ",");							/* parse the identifier */
		tempvtg.course_magnetic = strtok(NULL, ",");/* parse the course (magnetic) */
		strtok(NULL, ",");							/* parse the identifier */
		strtok(NULL, ",");							/* parse the speed in knots*/
		strtok(NULL, ",");							/* parse the identifier */
		tempvtg.speed_kmh = strtok(NULL, ",");		/* parse the speed in kilometers */
		//strtok(NULL, ",");						/* parse the identifier */
		memset(tempvtg.sentence, 0x0, strlen(tempvtg.sentence));
		memcpy(tempvtg.sentence, input, strlen(input));
		return (1);
	}
	return (0);
}

uint32_t CNmea::ParseGLL(char* input)
{
	if (memcmp(input, "GPGLL", 5) == NULL)
	{
		strtok(input, ",");							/* parse the header */
		tempgll.lat = strtok(NULL, ",");			/* parse the latitude */
		tempgll.lat_dir = strtok(NULL, ",");		/* parse the latitude direction */
		tempgll.lon = strtok(NULL, ",");			/* parse the longitude */
		tempgll.lon_dir = strtok(NULL, ",");		/* parse the longitude direction */
		strtok(NULL, ",");							/* parse the UTC */
		tempgll.status = strtok(NULL, ",");			/* parse the status */
		memset(tempgll.sentence, 0x0, strlen(tempgll.sentence));
		memcpy(tempgll.sentence, input, strlen(input));
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
		gga = tempgga;
		return;
	}
	else if (ParseRMC(input) == 1){
		rmc = temprmc;
		return;
	}
	else if (ParseVTG(input) == 1){
		vtg = tempvtg;
		return;
	}
	else if (ParseGLL(input) == 1){
		gll = tempgll;
		return;
	}
}

char* CNmea::GetNmeaPacket()
{
	return tempNMEAsentence;
}

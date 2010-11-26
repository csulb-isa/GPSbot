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
}

uint8_t CNmea::ParseGGA(char* input)
{
	if (memcmp((char*)input, "GPGGA", 5) == NULL)
	{
		strtok((char*)input, ",");				/* parse the header */
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
		return TRUE;
	}
	return FALSE;
}

uint8_t CNmea::ParseRMC(char* input)
{
 	if (memcmp((char*)input, "GPRMC", 5) == NULL)
	{
		strtok((char*)input, ",");				/* parse the header */
		temprmc.pos_stat = strtok(NULL, ",");	/* parse the UTC */
		temprmc.lat = strtok(NULL, ",");		/* parse the position status */
		temprmc.lat_dir = strtok(NULL, ",");	/* parse the latitude */
		temprmc.lon = strtok(NULL, ",");		/* parse the latitude direction */
		temprmc.lon_dir = strtok(NULL, ",");	/* parse the longitude */
		strtok(NULL, ",");						/* parse the longitude direction */
		temprmc.heading = strtok(NULL, ",");	/* parse the speed in knots */
		//strtok(NULL, ",");					/* parse the heading in degree (true) */
		//strtok(NULL, ",");					/* parse the date */
		//strtok(NULL, ",");					/* parse the magnetic variation in degrees (future)  */
		//strtok(NULL, ",");					/* parse the magnetic variation (future)*/
		return TRUE;
	}
	return FALSE;
}

uint8_t CNmea::ParseVTG(char* input)
{
	if (memcmp((char*)input, "GPVTG", 5) == NULL)
	{
		strtok((char*)input, ",");						/* parse the header */
		tempvtg.course_true = strtok(NULL, ",");		/* parse the course */
		strtok(NULL, ",");								/* parse the identifier */
		tempvtg.course_magnetic = strtok(NULL, ",");	/* parse the course (magnetic) */
		strtok(NULL, ",");								/* parse the identifier */
		strtok(NULL, ",");								/* parse the speed in knots*/
		strtok(NULL, ",");								/* parse the identifier */
		tempvtg.speed_kmh = strtok(NULL, ",");			/* parse the speed in kilometers */
		//strtok(NULL, ",");							/* parse the identifier */
		return TRUE;
	}
	return FALSE;
}

uint8_t CNmea::ParseGLL(char* input)
{
	if (memcmp((char*)input, "GPGLL", 5) == NULL)
	{
		strtok((char*)input, ",");						/* parse the header */
		tempgll.lat = strtok(NULL, ",");				/* parse the latitude */
		tempgll.lat_dir = strtok(NULL, ",");			/* parse the latitude direction */
		tempgll.lon = strtok(NULL, ",");				/* parse the longitude */
		tempgll.lon_dir = strtok(NULL, ",");			/* parse the longitude direction */
		strtok(NULL, ",");								/* parse the UTC */
		tempgll.status = strtok(NULL, ",");				/* parse the status */
		return TRUE;
	}
	return FALSE;
}

void CNmea::ParseNmeaPacket(uint8_t* input)
{
	// delete old contents and store whats new
	memset(tempNMEAsentence, 0x00, strlen((char*)tempNMEAsentence));
	memcpy(tempNMEAsentence, input, strlen((char*)input)+1);
	// look for a sentence match and parse
	if (ParseGGA((char*)input) == TRUE)
		return;
	else if (ParseRMC((char*)input) == TRUE)
		return;
	else if (ParseVTG((char*)input) == TRUE)
		return;
	else if (ParseGLL((char*)input) == TRUE)
		return;
}

uint8_t* CNmea::GetNmeaPacket()
{
	return tempNMEAsentence;
}

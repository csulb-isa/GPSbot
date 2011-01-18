/*
 *  ISA - Robo-Magellen	- Senior Project ET409B
 *  CSULB
 *  06-05-10
 *	sgrove
 *
 *
 *    NOTES:
 *		File for Debugging and controlling vehicle from the PC.  Build over Serial comm link
 *			that may then be passed to Xbee or the like.
 *
 *		Could use a method with a return type of char* but this will allow for single file fixing
 * 			of the packet format.  Otherwise every class file would then need to be updated - unless
 * 			a class of CDebug is formed that all others are derived from or friends with.
 *
 */

#include "pccomm.h"

extern DigitalOut led1;
extern DigitalOut led2;

extern CUart0 PC;
extern CUart2 Xbee;
extern CUart1 CMU;
extern CGps Gps;
extern CSonar Sonar1, Sonar2, Sonar3, Sonar4;
extern CSpeed TravelSpeed;
extern CServo Steer, Speed;
extern C2y0a700 Ir1, Ir2, Ir3;
extern CDigitalCompass Compass;

void BuildPcPacket()
{
	// Variables for parsing the incoming packets
	static char tpkt[256];
	static uint8_t cntr = 0;
	
	while(Xbee.IsReadable()){
		tpkt[cntr++] = Xbee.Read();
		// the last byte of a packet was found
		if(tpkt[cntr-1] == ']'){
			//PC.Write(tpkt);
			ParsePcPacket(tpkt);
			memset(tpkt, 0x00, strlen((char*)tpkt));
			cntr = 0;
		}
		else if(tpkt[cntr-1] == '['){
		 	if (cntr != 1){
			 	memset(tpkt, 0x00, strlen((char*)tpkt));
				cntr = 0;
				tpkt[cntr++] = '[';
			}
		}	
	}
}

void ParsePcPacket(char* input)
{
	//char data[16] = {NULL};
	// These are packet headers
	if (memcmp((char*)input, "[COMMAND", 7) == NULL){
		// ignore the packet header
		strtok((char*)input, "<");
		input = strtok(NULL, "]");
		// find out what peripheral to control
		if (memcmp((char*)input, "SPEED>", 6) == NULL){
			// ignore the command name
			strtok((char*)input, ">");
			// get the data from the packet
			input = strtok(NULL, "]");
			// convert from string to binary and pass to peripheral
			int result = atoi(input);
			led1 = !led1;
			//Speed.SetPos(result);
			Speed.UpdatePos(result);
			// send a response to the host
			Xbee.Write("[COMMAND< >OK]\n");		
		}
		else if (memcmp((char*)input, "STEER>", 6) == NULL){
			// ignore the command name
			strtok((char*)input, ">");
			// get the data from the packet
			input = strtok(NULL, "]");
			// convert from string to binary and pass to peripheral
			int result = atoi(input);
			led2 = !led2;
			//Steer.SetPos(result);
			Steer.UpdatePos(result);
			// send a response to the host
			Xbee.Write("[COMMAND< >OK]\n");
		}
	}
}

void TransmitVitals(void)
{
	char tmp_vital[256] = {NULL};
	// SONAR SENSORS
	sprintf(tmp_vital, "[STATUS<SONAR_L1>%d-in]\n", Sonar1.GetDistanceIn());
	Xbee.Write(tmp_vital);
	memset(tmp_vital, 0x00, strlen((char*)tmp_vital));
	
	sprintf(tmp_vital, "[STATUS<SONAR_L2>%d-in]\n", Sonar2.GetDistanceIn());
	Xbee.Write(tmp_vital);
	memset(tmp_vital, 0x00, strlen((char*)tmp_vital));

	sprintf(tmp_vital, "[STATUS<SONAR_R2>%d-in]\n", Sonar3.GetDistanceIn());
	Xbee.Write(tmp_vital);
	memset(tmp_vital, 0x00, strlen((char*)tmp_vital));

	sprintf(tmp_vital, "[STATUS<SONAR_R1>%d-in]\n", Sonar4.GetDistanceIn());
	Xbee.Write(tmp_vital);
	memset(tmp_vital, 0x00, strlen((char*)tmp_vital));
	// IR SENSORS	
	sprintf(tmp_vital, "[STATUS<IR1>%f-v]\n", Ir1.GetVoltage());	
	Xbee.Write(tmp_vital);
	memset(tmp_vital, 0x00, strlen((char*)tmp_vital));

	sprintf(tmp_vital, "[STATUS<IR2>%f-v]\n", Ir2.GetVoltage());	
	Xbee.Write(tmp_vital);
	memset(tmp_vital, 0x00, strlen((char*)tmp_vital));
	// SERVO STATUS
	sprintf(tmp_vital, "[STATUS<SERVO_STEER>%d]\n", Steer.GetPos());
	Xbee.Write(tmp_vital);
	memset(tmp_vital, 0x00, strlen((char*)tmp_vital));

	sprintf(tmp_vital, "[STATUS<SERVO_SPEED>%d]\n", Speed.GetPos());
	Xbee.Write(tmp_vital);
	memset(tmp_vital, 0x00, strlen((char*)tmp_vital));
	// MOVING SPEED
	sprintf(tmp_vital, "[STATUS<SPEED>%d-mph]\n", TravelSpeed.GetMph());	
	Xbee.Write(tmp_vital);
	memset(tmp_vital, 0x00, strlen((char*)tmp_vital));
	// COMPASS READINGS
	sprintf(tmp_vital, "[STATUS<HEADING-X>%d]\n", Compass.GetX());	
	Xbee.Write(tmp_vital);
	memset(tmp_vital, 0x00, strlen((char*)tmp_vital));

	sprintf(tmp_vital, "[STATUS<HEADING-Y>%d]\n", Compass.GetY());	
	Xbee.Write(tmp_vital);
	memset(tmp_vital, 0x00, strlen((char*)tmp_vital));

	sprintf(tmp_vital, "[STATUS<HEADING-Z>%d]\n", Compass.GetZ());	
	Xbee.Write(tmp_vital);
	memset(tmp_vital, 0x00, strlen((char*)tmp_vital));

	sprintf(tmp_vital, "[STATUS<HEADING>%d]\n", Compass.GetHeading());	
	Xbee.Write(tmp_vital);
	memset(tmp_vital, 0x00, strlen((char*)tmp_vital));
	// GPS READINGS
	sprintf(tmp_vital, "[STATUS<GPS-GGA>%s]\n", Gps.gga.sentence);	
	Xbee.Write(tmp_vital);
	memset(tmp_vital, 0x00, strlen((char*)tmp_vital));

	sprintf(tmp_vital, "[STATUS<GPS-VTG>%s]\n", Gps.vtg.sentence);	
	Xbee.Write(tmp_vital);
	memset(tmp_vital, 0x00, strlen((char*)tmp_vital));

	sprintf(tmp_vital, "[STATUS<GPS-RMC>%s]\n", Gps.rmc.sentence);	
	Xbee.Write(tmp_vital);
	memset(tmp_vital, 0x00, strlen((char*)tmp_vital));

	sprintf(tmp_vital, "[STATUS<GPS-GLL>%s]\n", Gps.gll.sentence);	
	Xbee.Write(tmp_vital);
	memset(tmp_vital, 0x00, strlen((char*)tmp_vital));
}

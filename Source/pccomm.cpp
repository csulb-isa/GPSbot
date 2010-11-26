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

extern CUart0 PC;
extern CUart2 Xbee;
extern CSonar Sonar1, Sonar2, Sonar3, Sonar4;
extern CSpeed TravelSpeed;
extern CServo Steer, Speed;
extern C2y0a700 Ir_Left, Ir_Right;

void BuildPcPacket()
{
	// Variables for parsing the incoming packets
	static char tpkt[256];
	static uint8_t cntr = 0;
	
	while(Xbee.IsReadable())
	{
	 	tpkt[cntr++] = Xbee.Read();
		// the last byte of a packet was found
		if(tpkt[cntr-1] == ']')
		{
			PC.Write(tpkt);
			ParsePcPacket(tpkt);
			memset(tpkt, 0x00, strlen((char*)tpkt));
			cntr = 0;
		}	
	}
}

void ParsePcPacket(char* input)
{
	char* decypher;
	if (memcmp((char*)input, "[CONFIG", 7) == NULL){
	 	Xbee.Write("CONFIG found\n");
	}
	else if (memcmp((char*)input, "[READ", 5) == NULL){
	   	Xbee.Write("READ found\n");
	}
	else if (memcmp((char*)input, "[MONITOR", 8) == NULL){
	 	// ignore the packet header
		strtok((char*)input, "?");
		// get the goods
		decypher = strtok(NULL, "?");
		// do what is requested
		if (memcmp((char*)decypher, "ENABLE", 6) == NULL)
			Xbee.Write("MONITOR Enabled\n");
		else if (memcmp((char*)decypher, "DISABLE", 6) == NULL)
			Xbee.Write("MONITOR Disabled\n");
	}
}

void TransmitStatus(void)
{
	char tmp[64] = {""};
	static int a = 0;
	static int b = 10;
	static int c = 20;
	static int d = 30;
	static int e = 40;
	static int f = 50;
	static int g = 60;
	static int h = 70;
	static int i = 80;
	a++;
	b++;
	c++;
	d++;
	e++;
	f++;
	g++;
	h++;
	i++;
	
	sprintf(tmp, "[STATUS<SONAR_L1>%d]\n", a);
	Xbee.Write(tmp);
	memset(tmp, 0x00, strlen((char*)tmp));
	
	sprintf(tmp, "[STATUS<SONAR_L2>%d]\n", b);
	Xbee.Write(tmp);
	memset(tmp, 0x00, strlen((char*)tmp));

	sprintf(tmp, "[STATUS<SONAR_R2>%d]\n", c);
	Xbee.Write(tmp);
	memset(tmp, 0x00, strlen((char*)tmp));

	sprintf(tmp, "[STATUS<SONAR_R1>%d]\n", d);
	Xbee.Write(tmp);
	memset(tmp, 0x00, strlen((char*)tmp));
		
	sprintf(tmp, "[STATUS<SERVO_POS>%d]\n", e);
	Xbee.Write(tmp);
	memset(tmp, 0x00, strlen((char*)tmp));

	sprintf(tmp, "[STATUS<SPEED_POS>%d]\n", f);
	Xbee.Write(tmp);
	memset(tmp, 0x00, strlen((char*)tmp));

	sprintf(tmp, "[STATUS<SPEED>%d]\n", g);	
	Xbee.Write(tmp);
	memset(tmp, 0x00, strlen((char*)tmp));

	sprintf(tmp, "[STATUS<IR1>%d]\n", h);	
	Xbee.Write(tmp);
	memset(tmp, 0x00, strlen((char*)tmp));

	sprintf(tmp, "[STATUS<IR2>%d]\n", i);	
	Xbee.Write(tmp);
	memset(tmp, 0x00, strlen((char*)tmp));
}

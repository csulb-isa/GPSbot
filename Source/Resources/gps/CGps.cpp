/*
 *  ISA - Robo-Magellen
 *  CSULB
 *  02-11-10
 *  sgrove
 *
 *    NOTES:
 *        	Using Delorme GPS2058-10 GPS module.  Vendor ID is $PDME (DME)
 *				Original baud is 4800.
 *
 *			Work on a better Serial class that will transmit from a buffer
 *				within the interrupt routine.
 *
 *			CSerial is done - an interrupt driven UART class that will handle
 *				all on chip UART peripherals.  Buffers TX and RX data until 
 *				program is ready or hardware is capable.
 *
 *			---ReStructure of this Class---
 *			CGPS should be derived from CSerial and have only one functions - 
 *				read and transmit data from an to the GPS receiver.  All nmea
 *				parsing should be handled within its own class, a class that works
 *				on arrays of data that are passed into it.
 */

#include "CGps.h"


CGps::CGps(PinName tx, PinName rx)	:	_data(tx,rx)
{
	wptr = 0;
	memset(tempPacket, 0x00, 255);
}

void CGps::Enable()
{
 	_data.Baud(4800);
	_data.Enable();
	//confignmeasentences(NMEA_BAUD_4800, (USE_GGA5 | USE_VTG | USE_GLL | USE_RMC), BROADCAST_ALWAYS);
	confignmeasentences(NMEA_BAUD_4800, (USE_GGA5), BROADCAST_ALWAYS);
	sendcoldstart();
}

void CGps::Disable()
{
	_data.Disable();
}

void CGps::Restart()
{
 	_data.Enable();
	sendhotstart();
}

void CGps::confignmeasentences(char* baud, uint8_t nmea, uint8_t broadcast)
{
	_data.Write("PDME,11,");
	_data.Write(baud);
	_data.Write(',');
	_data.Write(nmea);
	_data.Write(',');
	_data.Write(broadcast);
	_data.Write("\r\n");
}

void CGps::ChangeMaskAngle(char* input)
{
	_data.Write("$PDME,8,");
	_data.Write(input);
	_data.Write("\r\n");
}

void CGps::RequestPostitionError()
{
	_data.Write("$PDME,12,5\r\n");
}

void CGps::sendcoldstart()
{
	_data.Write("$PDME,0\r\n");
}

void CGps::sendwarmstart()
{
	_data.Write("$PDME,1\r\n");
}

void CGps::sendhotstart()
{
	_data.Write("$PDME,2\r\n");
}

void CGps::BuildPacket()
{
 	while(_data.IsReadable())
	{
	 	uint8_t scratch = _data.Read();
		// store a new packet
		if (scratch == '$'){
			CNmea::ParseNmeaPacket(tempPacket);
			memset(tempPacket, 0x00, 255);
			wptr = 0;
		}
		else
			tempPacket[wptr++] = scratch;
	}
}

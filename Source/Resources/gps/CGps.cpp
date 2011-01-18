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

#include "main.h"
#include "CGps.h"

extern CUart0 PC; 

CGps::CGps(PinName tx, PinName rx)	:	_data(tx, rx)
{
	wptr = 0;
	tempRawNmea.wptr = 0;
	memset(tempPacket, 0x00, TEMP_SENTENCE_SIZE);
	memset(tempRawNmea.data, 0x00, TEMP_RAW_NMEA_SIZE);
}

void CGps::Enable()
{
 	_data.Baud(4800);
	_data.Enable();
	confignmeasentences(NMEA_BAUD_4800, (USE_GGA5 | USE_VTG | USE_GLL | USE_RMC), BROADCAST_ALWAYS);
	//confignmeasentences(NMEA_BAUD_4800, (USE_GGA5), BROADCAST_ALWAYS);
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
	char tmp[64] = {NULL};
	sprintf(tmp, "$PDME,11,%s,%02x,%02x\r\n", baud, nmea, broadcast);
	_data.Write(tmp);
}

void CGps::ChangeMaskAngle(char* input)
{
	char* tmp = {NULL};
	sprintf(tmp, "$PDME,8,%s\r\n", input);
	_data.Write(tmp);
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
 	while(_data.IsReadable()){
	 	char scratch = _data.Read();
		// store a new packet
		StoreRawNmea(scratch);
		if (scratch == '$'){
			ParseNmeaPacket(tempPacket);
			memset(tempPacket, 0x00, 255);
			wptr = 0;
		}
		else{
			tempPacket[wptr++] = scratch;
		}
	}
}

void CGps::StoreRawNmea(char input)
{
	tempRawNmea.data[tempRawNmea.wptr++] = input;
	if (tempRawNmea.wptr >= TEMP_RAW_NMEA_SIZE){
		memset(tempRawNmea.data, 0x00, TEMP_RAW_NMEA_SIZE);
		tempRawNmea.wptr = 0;
	}	
}

void CGps::GetRawNmea(char* input)
{
 	memcpy(tempRawNmea.data, input, tempRawNmea.wptr);
	memset(tempRawNmea.data, 0x00, TEMP_RAW_NMEA_SIZE);
}



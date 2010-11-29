/*
 *  CSULB
 *  08-07-10
 *  sgrove
 *
 *    NOTES:
 *        	This class is responsible for reading data from the 
 *			Honeywell HMC5843 Digital Compass.
 *
 *			DONT ACK STATUS!!!!!
 */

#include "cdcompass.h"

CDigitalCompass::CDigitalCompass(PinName clk, PinName data)	:	_compass(clk, data)
{
	_compass.frequency(400000);	
}

void CDigitalCompass::Config()
{
	// set the condition
	_compass.start();
	// pass on the device address and a write command
	// this is the starting address
	_compass.write(0x3c);
	// Here is the data - 1 byte per address, auto incrementing
	_compass.write(0x02);
	_compass.write(0x00);
	_compass.stop();
}

void CDigitalCompass::CalculateHeading()
{
	// having troubles here - defective sensor??
	//rslt = atan2((float)10, (float)-10);
	rslt = atan2((float)data_y, (float)data_x);
	rslt = abs((rslt*(180/PI)));
	// need to work out quadrants based on z.y
	//if (rslt < 0){
	//	rslt = /*360+rslt;*/(180+rslt);
	//}
}

int32_t CDigitalCompass::GetHeading()
{
 	CalculateHeading();
	return rslt;
}

int32_t CDigitalCompass::GetX()
{
 	return data_x;
}

int32_t CDigitalCompass::GetY()
{
 	return data_y;
}
int32_t CDigitalCompass::GetZ()
{
 	return data_z;
}

uint32_t CDigitalCompass::GetStatus()
{
 	return status;
}

void CDigitalCompass::Read()
{
	_compass.start();
	_compass.write(0x3d);
	// get X data and store
	data_x = _compass.read(1);
	data_x = (data_x<<8);
	data_x |= (0x00ff&_compass.read(1));
	//get Y data and store
	data_y = _compass.read(1);
	data_y = (data_y<<8);
	data_y |= (0x00ff&_compass.read(1));
	// get Z data and store
	data_z = _compass.read(1);
	data_z = (data_z<<8);
	data_z |= (0x00ff&_compass.read(1));
	// get the status of the last read
	status = _compass.read(0);
	// initate a stop condition
	_compass.stop();
}

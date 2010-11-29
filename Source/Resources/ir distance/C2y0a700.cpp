/*
 *  ISA - Robo-Magellen
 *  CSULB
 *  02-27-10
 *	sgrove
 *
 *	IDE-Version:
 * 		µVision3 V3.90
 *
 *    NOTES:
 *        	Using Sharp GP2Y0A700K0F IR distance sensor module.
 *			Analog input voltage 0-Vcc+0.3 is the distance of an object.
 *				-Distance represented as an analog voltage
 *				-range is a decreasing  voltage from 5(close/hi)-20(far/lo) feet
 *
 *		EXAMPLE OF OPERATION:
 *			C2y0a700(PinName) Ir_Sensor;	// build the object from the default contrustor
 *			Ir_Sensor.Enable()				// prepare the module for use
 *			FT = Ir_Sensor.GetDistance();	// get the object distance
 *			V = Ir_Sensor.GetVoltage();		// get the amount of volatge measured on the pin
 *			AV = Ir_Sensor.GetAverage();	// get the sample average (a/d result)
 *			Ir.Sensor.Disable();			// turn off the module
 *
 */

#include "C2y0a700.h"

C2y0a700::C2y0a700(PinName pin)	: _distance(pin)
{
	// make sure we are filling the buffer at location 0
	result.wptr = temp = 0;
}

void C2y0a700::Enable()
{
	// attach the interrupt handler (valid sample every 2ms - read and average at 20ms)
	_elapse.attach(this, &C2y0a700::interrupt, .002); 
}

void C2y0a700::Disable()
{
	// detach the interrupt when we are not in need of this sensor
	_elapse.detach();
}

uint32_t C2y0a700::GetAverage()
{
	// pass the computed average to the main program
	return result.average;
}

void C2y0a700::computeaverage()
{
	// pull the data from the buffer and prepare for average 
	for(uint8_t cnt = 0; cnt < SAMPLE_CNT; cnt++)
	 	temp += result.data[cnt];

	// ensure we dont divide by 0 and crash the program
	if (temp == 0)
		result.average = 0;
	else
		result.average = temp/SAMPLE_CNT;
}

uint32_t C2y0a700::GetDistance()
{
 	// go and calculate the distance in ft.
	calculatedistance();			/* convert the a/d result to feet */
	// pass the formatted result to the user
	return (uint32_t)IR_DISTANCE[dist];
}

// need to work on these calculations from data sheet
void C2y0a700::calculatedistance()
{
//	if (result.average < )
//
//	else if (result.average <
//
//	else if (result.average < 
//
}

float C2y0a700::GetVoltage()
{
 	// return the voltage that is measured as it 
	return (result.data[result.wptr]*A_D_RESOLUTION);
}

float C2y0a700::GetAveraveVolatage()
{
 	return (result.average*A_D_RESOLUTION); 
}

void C2y0a700::interrupt()
{
	// load the sample into the averaging buffer and increment for next read
	result.data[result.wptr++] = _distance.read_u16();
	
	// 0-(n-1)  is equal to n size, reset the buffer locator for the next sample 
	if (result.wptr == (SAMPLE_CNT-1)){
		result.wptr = 0;
		computeaverage();
	}
}

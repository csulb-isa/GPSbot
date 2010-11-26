/*
 *  ISA - Robo-Magellen	- Senior Project ET409B
 *  CSULB
 *  03-07-10
 *	sgrove
 *
 *	IDE-Version:
 * 		µVision3 V3.90
 *
 *    NOTES:
 *        	Using SRF05 untrasonic distance sensor module.
 *			Digatal echo pulse input after sending a request for distance
 *				-Distance represnted as ~.5 ft. per 1mS
 *				-1mS-30mS - max distance is ~15 ft.
 *				-2 pins are used per derivation of this class, 1 input and 1 output
 *				- formulas used are: time(uS)/148 = distance (inches)
 *									 time(uS)/58 = distance (centimeters)
 *
 *		EXAMPLE OF USE:
 *			CSonar(PinName) Sensor_Left;	// build from the only constructor
 *			Sensor_Left.Enable();			// start the module and get readings
 *			uint8_t FT = Sensor_Left.GetDistance();		// return the result
 *			Sensor_Left.Disable();			// preserve uC power when not in use
 *
 */

#include "CSrf05.h"

CSonar::CSonar(PinName pinin, PinName pinout)	:	_echo(pinin), _trigger(pinout)
{
	// this will start the capture sequence
	_echo.rise(this, &CSonar::riseinterrupt);
	// must test same pin doing rise and fall interrupt - we are interested in
	// the time high of the pulse
	_echo.fall(this, &CSonar::fallinterrupt); 
	// trigger is activated on a falling edge (after rising)
	_trigger.write(0);
}

void CSonar::Enable()
{
	// attach a timed interrupt that will poll the sensor status and generate 
	// pings when necessary (~40mS)
	// ultrasonic wavelength must be dead before restarting
	_elapse.attach_us(this, &CSonar::elapseinterrupt, 400);
}

void CSonar::Disable()
{
	// detach the interrupt when idle
	_elapse.detach();
}

void CSonar::riseinterrupt()
{
 	// reset the distance pulse counter
	_risetime.reset();
	// start the distance pulse counter
	_risetime.start();
}

void CSonar::fallinterrupt()
{
 	// stop the distance pulse counter
	_risetime.stop();
	// store the distance calculation until called upon
	periodms = _risetime.read_ms();
	periodus = _risetime.read_us();
	// This works but seems like overkill, just calculate when the value
	// is needed by the main program
	//calculatedistance();	/* find out where the object is */
}

void CSonar::elapseinterrupt()
{
	// make sure the last ultrasonic burst has disipated 
	if (elapsecnt >= 100)
	{
	 	// 400uS entrance * 101 ~=40mS sensor refresh rate
		if (elapsecnt > 101)
			elapsecnt = 0;

		// the sensor pings on the falling edge, next time
		_trigger.write(1);
		elapsecnt++;
		return;
	}
	// drop the edge and cause a ping by the sensor
	_trigger.write(0);
	// increment the duration counter
	elapsecnt++;
}

void CSonar::calculatedistance(float input)
{
 	// distance is ~2mS for each foot of object distance
	distance = .5 * input;
}

uint8_t CSonar::GetDistance()
{
 	// give the user the last know complete sensor read
	//float tempperiodms = periodms;
	// go and calculate the distance based on the period
	//calculatedistance(tempperiodms);
	// return the distance
	//return distance;

	return (periodms/2);
}

uint8_t CSonar::GetDistanceIn()
{
 	// use the period to calculate the object distance
	// cosntant from datasheet
	return (periodus/148);
}

uint16_t CSonar::GetDistanceCm()
{
 	// use the period to calculate the object distance
	// constant from datasheet
	return (periodus/58);
}


// can be used but there is a better way...
//void CSonar::calculatedistance()
//{
// 	object.duration[object.wptr] = .5 * _risetime.read_ms();	  	/* distance is 2mS for each foot of object distance */
//	object.wptr++;
//
//	if (object.wptr > 1)
//	{
//		for (uint8_t cnt = 0; cnt < AMT; cnt++)
//		{
//			object.distance += object.duration[cnt];
//		}
//		
//		object.distance = object.distance/AMT;
//		object.wptr = 0;
//	}
//}

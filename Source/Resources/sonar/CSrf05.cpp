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
 *			CSonar Sensor_Left(PinName);	// build from the only constructor
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
	// re-initialize measurement variables
	periodms = periodus = 1;
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
}

void CSonar::elapseinterrupt()
{
	// make sure the last ultrasonic burst has elapsed
	// ISR is 400uS
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

uint32_t CSonar::GetPulsePeriodMs()
{
 	// Return the last known period measuremnent
	// from the sensor 
	return (periodms);
}

uint32_t CSonar::GetPulsePeriodUs()
{
 	// Return the last known period measuremnent
	// from the sensor	 
	return (periodus);
}

uint32_t CSonar::GetDistanceFt()
{
 	// use the period to calculate the object distance
	// cosntant from datasheet
	return (periodms/2);
}

uint32_t CSonar::GetDistanceIn()
{
 	// use the period to calculate the object distance
	// cosntant from datasheet
	return (periodus/148);
}

uint32_t CSonar::GetDistanceCm()
{
 	// use the period to calculate the object distance
	// constant from datasheet
	return (periodus/58);
}


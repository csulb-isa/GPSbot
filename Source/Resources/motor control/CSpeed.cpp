/*
 *  04-27-10
 *	sgrove
 *
 *    NOTES:
 *        	This should take the input interrupt and 
 *			calculate the period of the pulse to generate
 *			the bot speed.
 *
 *		EXAMPLE OF USE:
 *			CSpeed(PinName) Speed_Sensor;		// default and only constructor
 *			SpeedSensor.Enable();				// allow use of the sensor
 *			uin8_t MPH = SpeedSensor.GetMph();	// put the calculated speed into a variable
 *			SpeedSensor.Disable();			  	// stop the interrupt and the sensor use
 *
 *		04-10-10: Start of the file
 *		05-14-10: Verified calculations and module use
 *
 */

#include "CSpeed.h"

CSpeed::CSpeed(PinName pin)	:	_speed(pin), _indiacte(LED1)
{
	// reset the period before use
	periodms = 2000;
	// default state is pull down
	_speed.mode(PullUp);
	// use a led to indicate that the sensor is working
	_indiacte = 0;
}

void CSpeed::Enable()
{
	// attach the caputre interrupt
	_speed.rise(this, &CSpeed::riseinterrupt);
}

void CSpeed::Disable()
{
	// reset the counter and point to a trap interrupt
	_period.stop();
	_period.reset();
	periodms = 0;
	_speed.rise(this, &CSpeed::idleinterrupt);
}

void CSpeed::riseinterrupt()
{
	// stop the timer
	_period.stop();
	// load the period for later processing	when called
	periodms = _period.read_ms();
	// dont calculate the speed until it is asked for.  
	// This will reduce uC overhead
	// reset and restart the timer
	_period.reset();
	_period.start();
	// toggle the indicator LED
	_indiacte = !_indiacte;
}

void CSpeed::idleinterrupt()
{
	// just a trap for module off
}

void CSpeed::calculatemph(float input)
{	
	// 1879 ms = 1mph in our use.  Do not calculate speeds
	// lower than 1 mph (not floating point result)
	if ((input > MS_MPH) || (input < 1))
	{
		mph = 0;
		return;
	}
	// we must be moving
	// figure out the feet per second of travel
	mph = ROT_PER_FT/input;
	// and now the mile second
	mph = mph/FT_PER_MILE;
	// and finally miles per hour
	mph = mph*SEC_PER_HR;
	// and remove the offset from periodms being a decimal 
	// representation of the time in milli-seconds
	mph = mph*MS_PER_SECOND;
}

uint8_t CSpeed::GetMph()
{
	// use the value it last was incase of an interrupt
	// during this calculation
	float tempperiodms = periodms;
	// make the calculation only when called
	CSpeed::calculatemph(tempperiodms);
	return mph;
}

void CSpeed::calculatefph(float input)
{
	// 1879 ms = 1mph in our use.  Do not calculate speeds
	// lower than 1 mph (not floating point result)
	if (input > MS_MPH)
	{
		fph = 0;
		return;
	}
	// we must be moving
	// figure out the feet per second of travel
	fph = ROT_PER_FT/input;
	// and now the feet per hour
	fph = fph*SEC_PER_HR;
	// and remove the offset from periodms being a decimal
	fph = fph*MS_PER_SECOND;
}

uint32_t CSpeed::GetFph()
{
	// use the value it last was incase of an interrupt
	// during this calculation
	float tempperiodms = periodms;
	// make the calculation only when called
	CSpeed::calculatefph(tempperiodms);
	return fph;
}

void CSpeed::calculatemps(float input)
{	
	// 1879 ms = 1mph in our use.  Do not calculate speeds
	// lower than 1 mph (not floating point result)
	if (input > MS_MPH)
	{
		mps = 0;
		return;
	}
	// we must be moving
	// figure out the feet per second of travel
	mps = ROT_PER_FT/input;
	// and now the miles per second
	mps = mps/FT_PER_MILE;
	// and remove the offset from periodms being a decimal 
	// representation of the time in milli-seconds
	mps = mps*MS_PER_SECOND;
}

uint32_t CSpeed::GetMps()
{
	// use the value it last was incase of an interrupt
	// during this calculation
	float tempperiodms = periodms;
	// make the calculation only when called
	CSpeed::calculatemps(tempperiodms);
	return mps;
}

void CSpeed::calculatefps(float input)
{
	// 1879 ms = 1mph in our use.  Do not calculate speeds
	// lower than 1 mph (not floating point result)
	if (input > MS_MPH)
	{
		fps = 0;
		return;
	}
	// we must be moving
	// figure out the feet per second of travel
	fps = ROT_PER_FT/input;
	// representation of the time in milli-seconds
	fps = fps*MS_PER_SECOND;
}

uint32_t CSpeed::GetFps()
{
	// use the value it last was incase of an interrupt
	// during this calculation
	float tempperiodms = periodms;
	// make the calculation only when called
	CSpeed::calculatefps(tempperiodms);
	return fps;
}


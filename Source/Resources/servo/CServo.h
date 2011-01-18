#ifndef CSERVO_H
#define CSERVO_H

// peripheral library access
#include ".\mbedLibrary\mbed.h"

#define MAX_STEER_HEADING	180		/* measured by actual controller */
#define MIN_STEER_HEADING	0		/* measured by actual controller */
#define PWM_PERIOD			20.0	/* time in ms for PWM period */
#define PWM_ABS_CENTER		1.5		/* time in ms for 90 deg output */
#define PWM_ABS_LEFT		1.0		/* time in ms for 0 deg output */
#define PWM_ABS_RIGHT		2.0		/* time in ms for 180 deg output */
#define PWM_RESOLUTION	(.05/255)	/* full rotation width from center / 8bit input */
#define DEG_RESOLUTION	(.05/100)	/* 100 percent of a result */
#define PWM_OFFSET		(255+127)	/* resolution of result + center result */
#define DEG_OFFSET			100
#define HEADING_RANGE	(MAX_STEER_HEADING-MIN_STEER_HEADING)

class CServo
{
private:
	PwmOut _pwm;  				/* Derived object for servo control */
	Ticker _update;
	int8_t adj_val, set_val;
public:
	// constructor
	CServo(PinName);

	// angle methods for controlling 1.0 -> 2.0 mS pulses
	// based on range of variable passed.  
	// 127 = max, -127 = max and 0 = min
	int8_t GetPos();       		/* Verified 02-09-10 */
	void SetPos(int8_t);   		/* Verified 02-09-10 */
	void SetPos();
	void UpdatePos(int8_t);

	// parameter passed in degrees
	uint8_t GetPosDeg();		/* Verified 04-20-10 */
	void SetPosDeg(float);		/* Needs work */

	// percentage of time high methods
	float GetPulsePercentage();			/* Copied from verified steering */
	void SetPulsePercentage(float);		/* Copied from verified steering */
};

#endif

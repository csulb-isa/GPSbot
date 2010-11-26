#ifndef CSERVO_H
#define CSERVO_H

// give CServo access to convert functions
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
#define MOTOR_RESOLUTION	255		/* amount of bits in the input */
#define HEADING_RANGE	(MAX_STEER_HEADING-MIN_STEER_HEADING)

class CServo
{
private:
	PwmOut _pwm;  					/* Derived object for servo control */

public:
	// angle methods for controlling 1.0 -> 2.0 mS pulses
	int8_t GetAngle();       		/* Verified 02-09-10 */
	void SetAngle(int8_t);   		/* Verified 02-09-10 */

	// parameter passed in degrees
	uint8_t GetAngleDeg();			/* Verified 04-20-10 */
	void SetAngleDeg(float);		/* Needs work */

	// percentage of time high methods
	uint8_t GetPulseWidth();			/* Copied from verified steering */
	void SetPulseWidth(uint8_t);		/* Copied from verified steering */

	CServo(PinName);
};

#endif

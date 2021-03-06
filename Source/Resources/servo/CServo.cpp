/*
 *  IEEE - Natcar / RoboMagellen
 *  CSULB
 *  01-26-10
 *	sgrove
 *
 *	IDE-Version:
 * 		�Vision3 V3.90
 *
 *    NOTES:
 *        - servo control via PWM for steering & drive
 *
 *            - 1.5mS will give 090deg. output (STRAIGHT / STOP)
 *            - 1.0mS will give 000deg. output (LEFT / REVERSE)
 *            - 2.0mS will give 180deg. output (RIGHT / FOWARD)
 *                        ^
 *                        |
 *                        |
 *            <----------------------->
 *            0deg        90deg        180deg
 *            -127        0            127
 *            0xff        0x00         0x7f
 *                        0x80
 *            1.0mS       1.5mS        2.0mS
 *            Left        Cnt          Right
 * 			  Rev		  Stop		   Foward
 *
 *		- Motor speed control can also be derived from this 
 *			class.  0 is idle, -127 is full reverse and 
 *			127 is full speed ahead.  Must pause around 0
 *			when switching from foward to reverse and vice 
 *			versa.
 *
 *		  -PWM period cannot be different than the servo PWM
 *
 *		EXAMPLE OF USE:
 *			CServo(PinName) Speed;		// build from the only constructor
 *			Speed.Enable();				// start up module and prepare for use
 *			Speed.SetAngleDeg(90);		// pass an angle of valid heading (0 -> 180)
 *			Speed.SetAngle(xx);			// signed number positive representing ranges of
 *										// 90 - 0 deg. and negaitive numbers representing
 *										// ranges of 180 - 91 degrees.
 *			Speed.SetPulseWidth(xx);	// xx can be any value ranging from 0-255 and the 
 *										// output is equal to the percentage (xx/255)(100)
 *			uint8_t ANG = Speed.GetAngleDeg();	// read the position of the wheels in angle degrees
 *			int8_t ANG = Speed.GetAngle()		// read the angle of the wheels with respect 
 *												// to 90deg. = 0, 180deg. = -127 and 0deg. = 127
 *			uint8_t PULSE = Speed.GetPulseWidth()	// get the pulse width of the PWM output pin.
 */
 
#include "CServo.h"

CServo::CServo(PinName pin)	:	 _pwm(pin)
{
	adj_val = set_val = 0;
	// need to set the cycle period
	_pwm.period_ms(PWM_PERIOD);
	// Set the wheels straignt (pulse width)
	_pwm = (PWM_ABS_CENTER/PWM_PERIOD);
}


//
//  TAKE AND FLOAT (0.0 TO 1.0) AND CONVERT INTO A SIGNED CHAR (SIGN + 7bits) 
//	FOR CURRENT PWM	DUTY CYCLE.  DEFINES ALLOW FOR SINGLE LOCATION CHANGES IF NEEDED.  
//	
//	FORMULA IS AS FOLLOWS:
//		(% DUTY CYCLE / RESOLUTION) - (RETURN RESOLUTION + CENTER VALUE)
//
//
 
int8_t CServo::GetPos()
{
    float temp = _pwm.read();		/* get the PWM peripherals current value */
    temp = (temp/PWM_RESOLUTION);	/* return value divided by the resolution */
	temp -= PWM_OFFSET;				/* subtract the offset */
    if(temp < 0)
		temp--;						/* if the number is neg, offset for 0 */
	return (int8_t)temp;			/* typecast the result as a signed char */
}

//
// 	TAKE AND SIGNED CHAR (SIGN + 7bits) AND CONVERT INTO A FLOAT FROM 0.0 TO 1.0 
//	FOR PERCENTAGE OF PWM DUTY CYCLE.  DEFINES ALLOW FOR SINGLE LOCATION CHANGES 
//  IF NEEDED.  
//	
//	FORMULA IS AS FOLLOWS:
// 	
//		INPUT * RESOLUTION + CENTER DUTY CYCLE
// 		INPUT * ((ABS_RIGHT/PERIOD) - (ABS_LEFT/PERIOD)) + (ABS_CENTER/PERIOD)
//
//

void CServo::UpdatePos(int8_t input)
{
 	set_val = input;
	_update.attach_us(this, &CServo::SetPos, 2000);
}

void CServo::SetPos() 
{    
    if (adj_val == set_val){
		_update.detach();
	}
	else if (adj_val < set_val){
		adj_val++;
	}
	else if (adj_val > set_val){
		adj_val--;
	}
	
	if ((adj_val == 0) || (adj_val == 0x80)){    	/* we are at dead center, 90deg. */
        _pwm = (PWM_ABS_CENTER/PWM_PERIOD);		/* Set the wheels straignt (pulse width) */
    } 
    else{     					 
        _pwm = ( (adj_val*PWM_RESOLUTION) + 
		         (PWM_ABS_CENTER/PWM_PERIOD) );	/* angle*resolution + 90deg */
	}
}

void CServo::SetPos(int8_t input) 
{	
	if ((input == 0) || (input == 0x80)){    	/* we are at dead center, 90deg. */
        _pwm = (PWM_ABS_CENTER/PWM_PERIOD);		/* Set the wheels straignt (pulse width) */
    } 
    else{     					 
        _pwm = ( (input*PWM_RESOLUTION) + 
		         (PWM_ABS_CENTER/PWM_PERIOD) );	/* angle*resolution + 90deg */
	}
}

uint8_t CServo::GetPosDeg()
{
    float temp = _pwm.read();		/* get the PWM peripherals current value */
	temp = (temp/DEG_RESOLUTION);	/* return value divided by the resolution */
	temp -= DEG_OFFSET;				/* subtract the offset */
	// do not divide by 0
	if (temp == 0)
		temp = 0;
	else
		temp = temp/100;

	temp = temp*HEADING_RANGE;		/* Create the angle in deg */
	return (int8_t)temp;			/* typecast the result as a unsigned char */
}

void CServo::SetPosDeg(float input) 
{
	if ((input < MAX_STEER_HEADING) && (input > MIN_STEER_HEADING))
		_pwm = (((2.0)-(input/180))/PWM_PERIOD);
}

void CServo::SetPulsePercentage(float input)
{
	_pwm = input;
}

float CServo::GetPulsePercentage()
{
    return _pwm.read();
}



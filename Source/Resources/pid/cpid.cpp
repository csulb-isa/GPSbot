/*
 *	Class of for PID control of hardware - done in software
 *
 *		Example was given in EETimes (Oct2000) in an article
 * 		named "PID without a PhD.  Has been tweeked and optimized
 *		from original form.  Original author, Tim Wescott.
 *
 *			Gain Starting points are:
 *				p = 10
 *				i = 0.1
 *				d = 50
 *
 */

#include "cpid.h"

double CPid::UpdatePID(double err, double pos)
{
	// calculate the proportional term
	tempP = pid.p_gain * err;
	// store the integral state for next time
	pid.i_state += err;
	
	// calculate the integral state with appropriate limiting
	if (pid.i_state > pid.i_max) 
		pid.i_state = pid.i_max;		// if lager error than max, set as max
	else if (pid.i_state < pid.i_min) 
		pid.i_state = pid.i_min;		// if small error than min, set as min

	// calculate the integral term
	tempI = pid.i_gain * i_state;
	// current pos - last pos * gain
	tempD = pid.d_gain * (pos - pid.d_state);
	// store the last state
	pid.d_state = pos;

	return tempP + (tempI - tempD);
}

void CPid::SetGain(double p, double i, double d)
{
 	pid.p_gain = p;
	pid.i_gain = i;
	pid.d_gain = d;
}
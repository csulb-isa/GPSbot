#ifndef __C2Y0A700_H__
#define __C2Y0A700_H__

#include ".\mbedLibrary\mbed.h"

// # to sample before averaging
#define SAMPLE_CNT		10
#define A_D_RESOLUTION	.000050354		// Vcc/65535

// object that will be used while capturing data
typedef struct
{
	uint16_t data[SAMPLE_CNT];
	uint8_t wptr;
	uint16_t average; 
} DISTANCE_T;

// used for estimated distance returns
const uint8_t IR_DISTANCE[] = {0x0,0x1,0x2,0x3,0x4,0x5,0x6,0x7,0x8,0x9,0xa,0xb,0xc,0xd,0xe,0xf};

// class footprint and the inheritance of useful classes
class C2y0a700
{
private:
	// members that are going to be derived and used
	Ticker _elapse;
	AnalogIn _distance;
	DISTANCE_T result;
	
	// variables that are hands off to the user
	uint8_t dist;
	
	// this will be the average computed sample 
	uint32_t temp;	

	// methods that are hands off
	void computeaverage();			/* this builds the average */
	void setdist();			/* store the distance based on a/d result */
	void interrupt();		/* here is where the timeout interrupt will occur for the timer */

public:
	// access to the user
	void Enable();			/* enable the auto aquire */
	void Disable();			/* cancel the auto acquire */	
	uint16_t GetAverage();	/* return the computed average */
	uint8_t GetDistance();	/* return the formated distance */
	float GetVoltage();		/* the voltage measured on the analog pin */
	
	C2y0a700(PinName);		/* Constructor */

};


#endif

#ifndef CSRF05_H
#define CSRF05_H

// peripheral library access
#include ".\mbedLibrary\mbed.h"

class CSonar
{
private:
	// derived class members (mbed library)
	InterruptIn	_echo;
	Timer _risetime;
	DigitalOut _trigger;
	Ticker _elapse;
	
	// data types
	float distance;
	uint32_t periodms;
	uint32_t periodus;

	// interrupt
	uint8_t elapsecnt;

	// protected methods
	void riseinterrupt();	   		// start the timer class for distance
	void fallinterrupt();	  		// stop the timer class and get the distance
	void elapseinterrupt();	  		// elapse every 40mS (max distance is 32mS)
	
public:
	// constructor
	CSonar(PinName, PinName);		// tested and verified 04-06-10

	// user access
	void Enable();	   				// tested and verified 04-06-10
	void Disable();					// tested and verified 04-06-10
	uint32_t GetPulsePeriodMs();	// tested and verified 04-16-10
	uint32_t GetPulsePeriodUs();	// tested and verified 04-16-10
	uint32_t GetDistanceFt();		// tested and verified 04-16-10
	uint32_t GetDistanceIn();
	uint32_t GetDistanceCm();
};


#endif

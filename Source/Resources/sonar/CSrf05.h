#ifndef CSRF05_H
#define CSRF05_H

// give CSonar access to convert functions
#include ".\mbedLibrary\mbed.h"

// define used for averaging the results
#define AMT 	4

// variable used to store the data that is being worked on
typedef struct
{
 	uint8_t duration[AMT];
	uint8_t distance;
	uint8_t wptr;
}SONAR_T;

// The class and who it inherits (runtime inheritance is also done in the constructor)
class CSonar
{
private:
	// class members and derivitives
	InterruptIn	_echo;
	Timer _risetime;
	DigitalOut _trigger;
	Ticker _elapse;
	
	// data types
	SONAR_T object;		/* only used when averaging results */
	float distance;
	uint32_t periodms;
	uint32_t periodus;

	// interrupt
	uint8_t elapsecnt;

	// protected methods
	void riseinterrupt();	   		// start the timer class for distance
	void fallinterrupt();	  		// stop the timer class and get the distance
	void elapseinterrupt();	  		// elapse every 40mS (max distance is 32mS)
	void calculatedistance(float);	// temporary
	
public:
	// constructor
	CSonar(PinName, PinName);	// tested and verified 04-06-10

	// user access
	void Enable();	   			// tested and verified 04-06-10
	void Disable();				// tested and verified 04-06-10
	uint8_t GetDistance();		// tested and verified 04-16-10
	uint8_t GetDistanceIn();
	uint16_t GetDistanceCm();
};


#endif

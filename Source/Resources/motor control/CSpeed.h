#ifndef CSPEED_H
#define CSPEED_H

// peripheral library access
#include ".\mbedLibrary\mbed.h"

#ifndef TRUE
#define TRUE	1
#define FALSE	0
#endif

#define TIRE_CIRCUM		15.125
#define IN_PER_FT		12.000
#define ROT_PER_FT		(TIRE_CIRCUM/IN_PER_FT)
#define FT_PER_MILE		5280
#define SEC_PER_HR		3600
#define MS_MPH			1800
#define MS_PER_SECOND	1000

class CSpeed
{
private:
	// memeber of the Class by inheritance
	InterruptIn _speed;
	Timer _period;
	DigitalOut _indiacte;

	// variables
	float periodms;
	float mph, fph, mps, fps;

	// member intrrupt functions
	void riseinterrupt();
	void idleinterrupt();

	// member functions that are hands off
	void calculatemph(float);
	void calculatefph(float);
	void calculatemps(float);
	void calculatefps(float);

public:
	// default constructor need an IO pin
	CSpeed(PinName);
	
	// attach the interrupt and start monitoring the pin
	void Enable(void);
	void Disable(void);
	
	// user access to the pertinent & formatted data
	uint32_t GetMph(void);
	uint32_t GetFph(void);
	uint32_t GetMps(void);
	uint32_t GetFps(void);	
};

#endif

#ifndef CDCOMPASS_H
#define CDCOMPASS_H

// give access to peripheral library
#include ".\mbedLibrary\mbed.h"

#define PI	(double)3.1415926535897932384626433832795


// Defines for Memory of the device
#define HMC5843_WRITE		0x3C
#define HMC5843_READ		0x3D

#define HMC5843_CONFIG_A	0x00
#define HMC5843_CONFIG_B	0x01
#define HMC5843_MODE_RW		0x02
#define HMC5843_X_MSB		0x03
#define HMC5843_X_LSB		0x04
#define HMC5843_Y_MSB		0x05
#define HMC5843_Y_LSB		0x06
#define HMC5843_Z_MSB		0x07
#define HMC5843_Z_LSB		0x08
#define HMC5843_STATUS		0x09
#define HMC5843_ID_A		0x0a
#define HMC5843_ID_B		0x0b
#define HMC5843_ID_C		0x0c

// Configurations
#define BIAS_NORM 		(0x0<<0)
#define BIAS_POS		(0x1<<0)
#define BIAS_NEG		(0x2<<0)

#define OUTPUT_1_HZ		(0x1<<2)
#define OUTPUT_2_HZ		(0x2<<2)
#define OUTPUT_5_HZ		(0x3<<2)
#define OUTPUT_10_HZ	(0x4<<2)
#define OUTPUT_20_HZ	(0x5<<2)
#define OUTPUT_50_HZ	(0x6<<2)

#define GAIN_0_7		(0x0<<5)
#define GAIN_1_0		(0x1<<5)
#define GAIN_1_5		(0x2<<5)
#define GAIN_2_0		(0x3<<5)
#define GAIN_3_2		(0x4<<5)
#define GAIN_3_8		(0x5<<5)
#define GAIN_4_5		(0x6<<5)
#define GAIN_6_5		(0x7<<5)

#define MODE_CONT		(0x0<<0)
#define MODE_SINGLE		(0x0<<1)
#define MODE_IDLE		(0x0<<2)
#define MODE_SLEEP		(0x0<<3)

#define STATUS_RDY		(0x1<<0)
#define STATUS_LOCK		(0x1<<1)
#define STATUS_REN		(0x1<<2)

class CDigitalCompass
{
private:
	// peripheral members
	I2C _compass;
	Ticker _readtime;
	// variable members
	float rslt;
	int32_t data_x, data_y, data_z;
	uint32_t status;
	// method members
	void CalculateHeading();

public:
	// constructor
	CDigitalCompass(PinName, PinName);
	// called to setup the module
	void Config();
	void Enable();
	void Disable();
	// should  be called at an interval based on the config setting for 
	// the refresh rate
	void Read();
	// access to the calculated heading
	int32_t GetHeading();
	// access to the X reading
	int32_t GetX();
	// access to the Y reading
	int32_t GetY();
	// access to the Z reading
	int32_t GetZ();
	// access to the device status
	uint32_t GetStatus();
};


#endif

/*
 *	Author: sgrove
 *
 *	Project Notes:
 *		02-07-10 - Verified library import and CPU speed.  Starting to work on GPS class
 *					and project architecture.
 *
 *		02-16-10 - When using multiple PWM outputs the period must stay the same.
 *
 *		03-04-10 - using the printf method for serial objects halts the CPU until the 
 *					last character has been processed.  DO NOT USE IN MAIN CODE.  Look 
 *					at CGps for an example of a buffered tx use.  Also, Interrupt will
 *					not be generated by tx complete until a vaild rx byte occurs.
 *
 *		04-05-10 - Timeout Class only generates one interrupt after attached period.
 *					Ticker will always tick - generating interrupts until detached
 *					Timer will get the time from which a start and stop point are defined
 *					wait parameter is a float and 1 = 1sec.
 *					Note to self - when deriving multiple classes member varialbes (static)
 *					are not derived again by each class.  USE PRIVATE MEMBER VARIABLES
 *					
 *
 */

// Runtime configuration
#define _DEBUG_

// project dependant files
#include "main.h"

// Stuff for debugging
DigitalOut Status(LED4);
Ticker flip;
void fliphandler (void)
{
	Status = !Status;
}

// Global Object that are used throught the project.
// Mostly peripherals custom coded to interface with 
// sensors and controllers
// gerenal purpose debugging
CUart0 PC(USBTX, USBRX);
// Wireles transfers to PC
CUart2 Xbee(p28, p27);	
// Image processing for target seeking and possibly object avoidance
CUart1 CMU(p13, p14);
// Config the GPS receiver
CGps Gps(p17, p18);
// Coordinate system receiving - encapsulated inside of CGPS class
//CUart3 Gps(p17, p18);		

// steering and speed control
CServo Steer(p22);
CServo Speed(p24);

// sonar object detection
// sensor mounted in left center position (facing forward)
CSonar Sonar2(p5, p6);
// sensor mounted in right center position (facing forward)
CSonar Sonar3(p29, p30);
// sensor mounted in far left position (facing forward)
CSonar Sonar1(p7, p8);
// sensor mounter far right position (facing forward)
CSonar Sonar4(p21, p23);

// ir distance object detection
// connection in center of vehicle
C2y0a700 Ir2(p16);
// connection on left side of vehicle
C2y0a700 Ir1(p19);
// connection on right side of vehicle
C2y0a700 Ir3(p15);

// calculation of the bots speed
CSpeed TravelSpeed(p12);

// Compass heading from Honeywell device  - seems to be having trouble
CDigitalCompass Compass(p9, p10);

// Tickers for handling events that that are embedded within classes
// and would otherwise need to be called by the application
Ticker GpsParseTic;		// enable and disable with Gps
Ticker PcCommParseTic;	// enable and disable with Xbee
Ticker SendStatus;		// update pc with run status

// start of the runtime application
int main() 
{	
	flip.attach(&fliphandler, 0.125);
	// start the PC connection, camera and GPS
	Xbee.Baud(9600);
	Xbee.Enable();

	CMU.Baud(9600);
	CMU.Enable();

	PC.Baud(9600);
	PC.Enable();

	// start the GPS receiver and look for packets every
	// 250 mili-seconds.  Time can be adjusted based on amount
	// of sentences that are enabled
	Gps.Enable();
	GpsParseTic.attach(&Gps, &CGps::BuildPacket, 0.250);
	
	// configure all the sensors on the car
	// these are the 4 sonar sensors
	// Make controlable via PC interface at a later time
	Sonar1.Enable();
	Sonar2.Enable();
	Sonar3.Enable();
	Sonar4.Enable();

	// Start the IR sensors for use
	// Make controlable via PC interface at a later time
	Ir1.Enable();
	Ir2.Enable();
	Ir3.Disable();	// currently not on the vehicle

	// no need to set up the PWM objects - Speed and steering
	// These are always running
	//Speed();
	//Steer();

	// This is the object that will generate the MPH reading from the 
	// onboard sensor.  LED1 is used to visualize the speed pulse
	TravelSpeed.Enable();

	// indicate that we are moving on and prepare for 
	// incoming packets of data.  These packets are from the 
	// Xbee module
	PcCommParseTic.attach(&BuildPcPacket, 1.0);
	SendStatus.attach(&TransmitStatus, 3.0);

	// Connect to compass and start getting data
	Compass.Config();

	// Indicate that the boot process is complete and entering main
	// control code
	Xbee.Write(" Hardware configuration complete, application version 1a \n");
	
	// here is the main program
	while(1)
	{
	 	wait(1);
		Compass.Read();
	}
}


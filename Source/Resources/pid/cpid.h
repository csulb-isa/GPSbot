#ifndef CPID_H
#define CPID_H

typedef struct
{
	double d_state;		// last position input
	double i_state;		// integrator state
	double i_min;
	double i_max;

	// gain for correction
	double i_gain;		// integral gain
	double p_gain;		// proportional gain
	double d_gain;		// derivitive gain
} PID_T;

class CPid
{
private:
	// variables used in calculations
	double tempP, tempI, tempD;

protected:
	PID_T pid;

public:
	CPid();
	double Update(double, double);
	SetGain(double, double, double);
};

#endif
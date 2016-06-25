/*  CheapStepper.h - 
	Library for the 28BYJ-48 stepper motor, using ULN2003 driver board
	https://arduino-info.wikispaces.com/SmallSteppers

	Library written by Tyler Henry, 6/24/2016

	uses 8-step sequence: A-AB-B-BC-C-CD-D-DA

	motor has gear ratio of either:
		64:1 (per manufacturer specs)
		or 
		63.68395:1 measured
			(see: http://forum.arduino.cc/index.php?topic=71964.15)
	* 64 steps per internal motor rev
	= 

	4096 total mini-steps / revolution
	or ~4076 (4075.7728) depending on exact gear ratio

	assumes 5v power source for rpm calc
*/

#ifndef CHEAPSTEPPER_H
#define CHEAPSTEPPER_H

#include "Arduino.h"

class CheapStepper
{

public: 
	CheapStepper();
	CheapStepper (int in1, int in2, int in3, int in4);

	void setRpm (int rpm); // sets speed (10-24 rpm, hi-low torque)
	// <6 rpm blocked in code, may overheat
	// 23-24rpm may skip

	void set4076StepMode() { totalSteps = 4076; }
	void setTotalSteps (int numSteps) { totalSteps = numSteps; }
	// allows custom # of steps (usually 4076)

	// blocking! (pauses arduino until move is done)
	void move (bool clockwise, int numSteps); // 4096 steps = 1 revolution
	void moveTo (bool clockwise, int toStep); // move to specific step position
	void moveDegrees (bool clockwise, int deg);
	void moveToDegree (bool clockwise, int deg);

	void moveCW (int numSteps) { move (true, numSteps); }
	void moveCCW (int numSteps) { move (false, numSteps); }
	void moveToCW (int toStep) { moveTo (true, toStep); }
	void moveToCCW (int toStep) { moveTo (false, toStep); }
	void moveDegreesCW (int deg) { moveDegrees (true, deg); }
	void moveDegreesCCW (int deg) { moveDegrees (false, deg); }
	void moveToDegreeCW (int deg) { moveToDegree (true, deg); }
	void moveToDegreeCCW (int deg) { moveToDegree (false, deg); }

	// to do: non-blocking versions of move()

	void step (bool clockwise);
	// move 1 step clockwise or counter-clockwise
	void stepCW () { step (true); } // move 1 step clockwise
	void stepCCW () { step (false); } // move 1 step counter-clockwise

	int getStep() { return stepN; } // returns current miniStep position
	int getDelay() { return delay; } // returns current delay (microseconds)

private:

	int calcDelay(int rpm); // calcs microsecond step delay for given rpm

	void seqCW();
	void seqCCW();
	void seq(int seqNum); // send specific sequence num to driver

	int pins[4] = {8,9,10,11}; // in1, in2, in3, in4

	int stepN = 0; // keeps track of step position
	// 0-4095 (4096 mini-steps / revolution) or maybe 4076...
	int totalSteps = 4096;

	int delay = 900; // microsecond delay between steps
	// 900 ~= 16.25 rpm
	// low speed (high torque) = 1465 ~= 1 rpm
	// high speed (low torque) = 600 ~=  24 rpm

	int seqN = -1; // keeps track of sequence number
};

#endif
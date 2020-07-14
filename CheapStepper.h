#ifndef CHEAPSTEPPER_H
#define CHEAPSTEPPER_H

#include "Arduino.h"

class CheapStepper
{

public: 
	CheapStepper(int in1, int in2, int in3, int in4);

	// Rotations per minute
	// <6 rpm blocked in code, may overheat
	// 23-24rpm may skip
	void setRpm(int value) {
		rpm = value;
		calcDelay();
	}

	// Steps per rotation
	void setSpr(int value) {spr = value;}

	void moveCW(uint32_t value);           // Move spesified steps clockwise
	void moveCCW(uint32_t value);          // Move spesified steps counterclockwise
	void moveDegreesCW(uint32_t value);    // 
	void moveDegreesCCW(uint32_t value);   // 
	void moveTo(uint32_t value);           // Move to spesified position clockwise
	void moveToDegree(uint32_t value);     // 

	void run();
	void stop();
	void off();

	void stepCW();                         // move 1 step clockwise
	void stepCCW();                        // move 1 step counter-clockwise

	int getPosition()  const {return position;}  // returns current miniStep position
	int getDelay()     const {return delay;}     // returns current delay (microseconds)
	int getStepsLeft() const {return stepsLeft;} // returns steps left in current move
	int getRpm()       const {return rpm;}
	int getSpr()       const {return spr;}

	// returns true if moving done
	bool isReady() const {
		return stepsLeft == 0;
	}

	// set virtual limit switches
	void setLimits(uint32_t cw, uint32_t ccw) {
		if (cw > ccw) {
			limitCW = cw;
			limitCCW = ccw;
		}
	}

	uint16_t interval() const {return delay;}

private:
	int pins[4];            // defaults to pins {8,9,10,11} (in1,in2,in3,in4 on the driver board)
	int direction;          // rotation direction
	uint32_t position;      // current position in steps
	uint16_t spr;           // steps per revolution
	uint16_t rpm;           // revolutions per minute
	int32_t limitCW;        // virtual limit switch for CW
	int32_t limitCCW;       // virtual limit switch for CCW
	uint8_t stepMask;       // microstep mask
	uint32_t lastStepTime;  // time in microseconds that last step happened
	uint32_t stepsLeft;     // steps left to move, neg for counter-clockwise
	int delay;              // microsecond delay between steps
	void calcDelay();       // calcs microsecond step delay for given rpm
	void step();            // send step sequence to driver

};

#endif

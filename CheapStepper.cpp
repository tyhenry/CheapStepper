/*  CheapStepper.cpp - 
	v0.2
	Library for the 28BYJ-48 stepper motor, using ULN2003 driver board
	https://arduino-info.wikispaces.com/SmallSteppers

	Library written by Tyler Henry, 6/2016

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


#include "Arduino.h"
#include "CheapStepper.h"


CheapStepper::CheapStepper () : pins({8,9,10,11}) {
	for (int pin=0; pin<4; pin++){
		pinMode(pins[pin], OUTPUT);
	}
}

CheapStepper::CheapStepper (int in1, int in2, int in3, int in4) : pins({in1,in2,in3,in4}) {
	for (int pin=0; pin<4; pin++){
		pinMode(pins[pin], OUTPUT);
	}
}

void CheapStepper::setRpm (int rpm){

	delay = calcDelay(rpm);
}

void CheapStepper::move (bool clockwise, int numSteps){

	for (int n=0; n<numSteps; n++){
		step(clockwise);
	}
}

void CheapStepper::moveTo (bool clockwise, int toStep){

	// keep to 0-(totalSteps-1) range
	if (toStep >= totalSteps) toStep %= totalSteps;
	else if (toStep < 0) {
		toStep %= totalSteps; // returns negative if toStep not multiple of totalSteps
		if (toStep < 0) toStep += totalSteps; // shift into 0-(totalSteps-1) range
	}
	while (stepN != toStep){
		step(clockwise);
	}
}

void CheapStepper::moveDegrees (bool clockwise, int deg){

	int nSteps = (unsigned long) deg * totalSteps / 360;
	move(clockwise, nSteps);
}

void CheapStepper::moveToDegree (bool clockwise, int deg){

	// keep to 0-359 range
	if (deg >= 360) deg %= 360;
	else if (deg < 0) {
		deg %= 360; // returns negative if deg not multiple of 360
		if (deg < 0) deg += 360; // shift into 0-359 range
	}

	int toStep = deg * totalSteps / 360;
	moveTo (clockwise, toStep);
}


// NON-BLOCKING MOVES

void CheapStepper::newMove (bool clockwise, int numSteps){

	// numSteps sign ignored
	// stepsLeft signed positive if clockwise, neg if ccw

	if (clockwise) stepsLeft = abs(numSteps);
	else stepsLeft = -1 * abs(numSteps);

	lastStepTime = micros();
}

void CheapStepper::newMoveTo (bool clockwise, int toStep){

	// keep toStep in 0-(totalSteps-1) range
	if (toStep >= totalSteps) toStep %= totalSteps;
	else if (toStep < 0) {
		toStep %= totalSteps; // returns negative if toStep not multiple of totalSteps
		if (toStep < 0) toStep += totalSteps; // shift into 0-(totalSteps-1) range
	}

	if (clockwise) stepsLeft = abs(toStep - stepN);
	// clockwise: simple diff, always pos
	else stepsLeft = -1*(totalSteps - abs(toStep - stepN));
	// counter-clockwise: totalSteps - diff, made neg

	lastStepTime = micros();
}

void CheapStepper::newMoveDegrees (bool clockwise, int deg){

	int nSteps = (unsigned long) deg * totalSteps / 360;
	newMove (clockwise, nSteps);
}

void CheapStepper::newMoveToDegree (bool clockwise, int deg){

	// keep to 0-359 range
	if (deg >= 360) deg %= 360;
	else if (deg < 0) {
		deg %= 360; // returns negative if deg not multiple of 360
		if (deg < 0) deg += 360; // shift into 0-359 range
	}

	int toStep = deg * totalSteps / 360;
	newMoveTo (clockwise, toStep);
}

void CheapStepper::run(){

	if (micros() - lastStepTime >= delay) { // if time for step
		if (stepsLeft > 0) { // clockwise
			stepCW();
			stepsLeft--;
		} else if (stepsLeft < 0){ // counter-clockwise
			stepCCW();
			stepsLeft++;
		} 

		lastStepTime = micros();
	}
}

void CheapStepper::stop(){

	stepsLeft = 0;
}


void CheapStepper::step(bool clockwise){

	if (clockwise) seqCW();
	else seqCCW();
}


/////////////
// PRIVATE //
/////////////

int CheapStepper::calcDelay (int rpm){

	if (rpm < 6) return delay; // will overheat, no change
	else if (rpm >= 24) return 600; // highest speed

	unsigned long d = 60000000 / (totalSteps* (unsigned long) rpm);
	// in range: 600-1465 microseconds (24-1 rpm)
	return (int) d;

}

int CheapStepper::calcRpm (int _delay){

	unsigned long rpm = 60000000 / (unsigned long) _delay / totalSteps;
	return (int) rpm;
	
}

void CheapStepper::seqCW (){
	seqN++;
	if (seqN > 7) seqN = 0; // roll over to A seq
	seq(seqN);

	stepN++; // track miniSteps
	if (stepN >= totalSteps){
		stepN -=totalSteps; // keep stepN within 0-(totalSteps-1)
	}
}

void CheapStepper::seqCCW (){
	seqN--;
	if (seqN < 0) seqN = 7; // roll over to DA seq
	seq(seqN);

	stepN--; // track miniSteps
	if (stepN < 0){
		stepN +=totalSteps; // keep stepN within 0-(totalSteps-1)
	}
}

void CheapStepper::seq (int seqNum){

	int pattern[4];
	// A,B,C,D HIGH/LOW pattern to write to driver board
	
	switch(seqNum){
		case 0:
		{
			pattern[0] = 1;
			pattern[1] = 0;
			pattern[2] = 0;
			pattern[3] = 0;
			break;
		}
		case 1:
		{
			pattern[0] = 1;
			pattern[1] = 1;
			pattern[2] = 0;
			pattern[3] = 0;
			break;
		}
		case 2:
		{
			pattern[0] = 0;
			pattern[1] = 1;
			pattern[2] = 0;
			pattern[3] = 0;
			break;
		}
		case 3:
		{
			pattern[0] = 0;
			pattern[1] = 1;
			pattern[2] = 1;
			pattern[3] = 0;
			break;
		}	
		case 4:
		{
			pattern[0] = 0;
			pattern[1] = 0;
			pattern[2] = 1;
			pattern[3] = 0;
			break;
		}
		case 5:
		{
			pattern[0] = 0;
			pattern[1] = 0;
			pattern[2] = 1;
			pattern[3] = 1;
			break;
		}
		case 6:
		{
			pattern[0] = 0;
			pattern[1] = 0;
			pattern[2] = 0;
			pattern[3] = 1;
			break;
		}
		case 7:
		{
			pattern[0] = 1;
			pattern[1] = 0;
			pattern[2] = 0;
			pattern[3] = 1;
			break;
		}
		default:
		{
			pattern[0] = 0;
			pattern[1] = 0;
			pattern[2] = 0;
			pattern[3] = 0;
			break;
		}
	}

	// write pattern to pins
	for (int p=0; p<4; p++){
		digitalWrite(pins[p], pattern[p]);
	}
	delayMicroseconds(delay);
}



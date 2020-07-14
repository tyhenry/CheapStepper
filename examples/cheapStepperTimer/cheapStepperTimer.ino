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


CheapStepper::CheapStepper (int in1, int in2, int in3, int in4) 
	: pins({in1,in2,in3,in4})
	, direction(0)
	, position(0)
	, spr(4096)
	, rpm(10)
	, limitCW(UINT32_MAX)
	, limitCCW(0)
	, stepMask(1)
	, lastStepTime(0)
	, stepsLeft(0)
{
	for (int pin=0; pin<4; pin++) {
		pinMode(pins[pin], OUTPUT);
		digitalWrite(pins[pin], LOW);
	}
}

void CheapStepper::moveCW(uint32_t value)
{
	if (isReady()) {
		direction = 1;
		stepsLeft = value;
		lastStepTime = micros();
	}
}

void CheapStepper::moveCCW(uint32_t value)
{
	if (isReady()) {
		direction = -1;
		stepsLeft = value;
		lastStepTime = micros();
	}
}

void CheapStepper::moveDegreesCW(uint32_t deg)
{
	moveCW(uint64_t(deg) * spr / 360);
}

void CheapStepper::moveDegreesCCW(uint32_t deg)
{
	moveCCW(uint64_t(deg) * spr / 360);
}

void CheapStepper::moveTo(uint32_t value)
{
	if (value > position) {
		moveCW(value - position);
	} else if (value < position) {
		moveCCW(position - value);
	}
}

void CheapStepper::moveToDegree (uint32_t value)
{
	uint32_t currentValue = uint64_t(position) * 360 / spr;
	if (value > currentValue) {
		moveDegreesCW(value - currentValue);
	} else if (value < currentValue) {
		moveDegreesCCW(currentValue - value);
	}
}

void CheapStepper::run(){
	if (stepsLeft > 0) {
		uint32_t currentTime = micros();
		if ((currentTime - lastStepTime) >= delay) {
			step();
			stepsLeft--;
			lastStepTime = currentTime;
		}
	}
}

void CheapStepper::stop()
{
	stepsLeft = 0;
}

void CheapStepper::off()
{
	for (int i=0; i<4; i++) digitalWrite(pins[i], 0);
}

void CheapStepper::stepCW() {
	direction = 1;
	step();
}

void CheapStepper::stepCCW() {
	direction = -1;
	step();
}

// PRIVATE

void CheapStepper::calcDelay()
{
	if (rpm < 6) return;   // will overheat, no change
	if (rpm >= 24) return; // highest speed
	delay = (uint64_t) 60000000 / ((uint64_t) rpm * (uint64_t) spr);
}

void CheapStepper::step()
{
	const uint8_t pattern[4] = {
		0b10000011, 
		0b00111000,
		0b00001110,
		0b11100000
	};

	position += direction;
	if (position >= limitCW) return;
	if (position <= limitCCW) return;

	// write pattern to pins
	for (int i = 0; i < 4; i++)  digitalWrite(pins[i], pattern[i] & stepMask);
	
	// prepere the bitmask for the next step
	if (direction > 0){
		stepMask <<= 1;
		if (stepMask == 0) stepMask = 0b00000001;
	} else if (direction < 0) {
		stepMask >>= 1;
		if (stepMask == 0) stepMask = 0b10000000;
	}
	
}

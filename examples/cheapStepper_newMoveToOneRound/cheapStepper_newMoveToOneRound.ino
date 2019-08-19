/*
 cheapStepper_newMoveToOneRound.ino
 ///////////////////////////////////////////
 using CheapStepper Arduino library v.0.2.0
 created by Marian Soban, 8/2019
 ///////////////////////////////////////////

 This sketch illustrates the library's
 "non-blocking" move functions within one rotation of stepper motor  -
 i.e. you can perform moves with the stepper over time
 while still running other code in your loop()

 This can be useful if your Arduino is multi-tasking,
 but be careful: if the other code in your loop()
 slows down your Arduino, the stepper motor may
 slow down or move with a stutter

 //////////////////////////////////////////////////////
 */

// first, include the library :)
#include <CheapStepper.h>

// next, declare the stepper
// and connect pins 8,9,10,11 to IN1,IN2,IN3,IN4 on ULN2003 board

CheapStepper stepper (8, 9, 10, 11);
// CheapStepper stepper(54, 55, 56, 57); // ArduPilot 2.5 case for A0-A3 pins

// let's also create a boolean variable to save the direction of our rotation
// and a timer variable to keep track of move times

unsigned long moveStartTime = 0; // this will save the time (millis()) when we started each new move

int angle = 0;

void setup() {
    // let's run the stepper at 12rpm (if using 5V power) - the default is ~16 rpm

    stepper.setRpm(12);

    // let's print out the RPM to make sure the setting worked
    Serial.begin(9600);
    Serial.println("-----------------------------------------");
    Serial.print("stepper RPM: ");
    Serial.print(stepper.getRpm());
    Serial.println();

    // and let's print the delay time (in microseconds) between each step
    // the delay is based on the RPM setting:
    // it's how long the stepper will wait before each step

    Serial.print("stepper delay (micros): ");
    Serial.println(stepper.getDelay());

    Serial.println("### MOVE STEPPER TO INITIAL POSITION MANUALLY - you have 5 seconds left");
    stepper.off();
    stepper.stop();
    delay(5000);
    Serial.println("### STARTING MOVEMENT");

    Serial.println();

    // now let's set up our first move...
    // let's move a half rotation from the start point
    int firstStepTo = 2048;
    // if isInitPosInHalfMove parameter is set to true, initial stepper position is right in the middle of movement
    stepper.newMoveToWithinOneRound(2048, true);
    Serial.print("moving to step: ");
    Serial.print(firstStepTo);
    Serial.print("\t steps left: ");
    Serial.println(stepper.getStepsLeft()); 

    moveStartTime = millis(); // let's save the time at which we started this move
}

void loop() {
    unsigned long now = millis();

    stepper.run();

    if (now - moveStartTime >= 3500) {
        moveStartTime = now;
        angle = (angle + 45) % 360;
        stepper.newMoveToDegreeWithinOneRound(angle, true);
        Serial.print("moving to angle: ");
        Serial.print(angle);
        Serial.print("\t steps left: ");
        Serial.println(stepper.getStepsLeft());
    }
}


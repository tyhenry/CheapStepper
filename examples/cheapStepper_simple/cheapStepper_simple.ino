/*
 * cheapStepper_simple.ino
 * ///////////////////////////////////////////
 * using CheapStepper Arduino library v.0.2.0
 * created by Tyler Henry, 7/2016
 * ///////////////////////////////////////////
 * 
 * this sketch illustrates basic step() functionality of the library:
 * the stepper performs a full rotation, pauses 1 second,
 * then does a full rotation in the other direction, and so on
 * 
 * //////////////////////////////////////////////////////
 */

// first, include the library :)

#include <CheapStepper.h>


CheapStepper stepper;
// here we declare our stepper using default pins:
// arduino pin <--> pins on ULN2003 board:
// 8 <--> IN1
// 9 <--> IN2
// 10 <--> IN3
// 11 <--> IN4

 // let's create a boolean variable to save the direction of our rotation

boolean moveClockwise = true;


void setup() {
  
  // let's just set up a serial connection and test print to the console
  
  Serial.begin(9600);
  Serial.println("Ready to start moving!");
}

void loop() {

  // let's move a full rotation (4096 mini-steps)
  // we'll go step-by-step using the step() function
  
  for (int s=0; s<4096; s++){
    // this will loop 4096 times
    // 4096 steps = full rotation using default values
    /* Note:
     * you could alternatively use 4076 steps... 
     * if you think your 28BYJ-48 stepper's internal gear ratio is 63.68395:1 (measured) rather than 64:1 (advertised)
     * for more info, see: http://forum.arduino.cc/index.php?topic=71964.15)
     */

    // let's move one "step" (of the 4096 per full rotation)
    
    stepper.step(moveClockwise);
    /* the direction is based on moveClockwise boolean:
     * true for clockwise, false for counter-clockwise
     * -- you could also say stepper.stepCW(); or stepper.stepCCW();
     */

    // now let's get the current step position of motor
    
    int nStep = stepper.getStep();

    // and if it's divisible by 64...
    
    if (nStep%64==0){ 

      // let's print the position to the console
      
      Serial.print("current step position: "); Serial.print(nStep);
      Serial.println();
      
    }
  }

  // now we've moved 4096 steps
  
  // let's wait one second
  
  delay(1000);

  // and switch directions before starting loop() again
  
  moveClockwise = !moveClockwise;
}



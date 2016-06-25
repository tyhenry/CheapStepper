#include <CheapStepper.h>

// this sketch performs a full rotation, pauses 1 second, then does a full rotation in the other direction, and so on

CheapStepper stepper;
// declare your stepper using default pins:
// arduino pin <--> pins on ULN2003 board:
// 8 <--> IN1
// 9 <--> IN2
// 10 <--> IN3
// 11 <--> IN4

boolean moveClockwise = true;

void setup() {
  Serial.begin(9600);
}

void loop() {
  
  for (int s=0; s<4096; s++){
    // loop 4096 times
    // 4096 steps = full rotation (maybe 4076... depends on your exact gear ratio: http://forum.arduino.cc/index.php?topic=71964.15)
    
    stepper.step(moveClockwise);
    // step once (direction based on moveClockwise boolean)
    // note: you can also say stepper.stepCW(); or stepper.stepCCW();
    
    int nStep = stepper.getStep(); // get current step position of motor
    
    if (nStep%64==0){ // if it's divisible by 64...
      Serial.print("step: ");
      Serial.print(nStep); //... print to console
      Serial.println();
    }
  }
  delay(1000); // wait a sec
  
  moveClockwise = !moveClockwise; // switch directions
}



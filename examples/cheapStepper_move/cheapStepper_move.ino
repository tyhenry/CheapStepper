#include <CheapStepper.h>

// this sketch shows how to set rpm
// and shows a few different types of move functions - by steps or by degrees


CheapStepper stepper (8,9,10,11); 
// declare your 28YBJ-48 stepper motor with ULN2003 driver board
// on pins 8,9,10,11 -> IN1,IN2,IN3,IN4 on board


boolean moveClockwise;

void setup() {
  
  stepper.setRpm(20); 
  // let's set a custom speed of 20rpm (the default is ~16.25rpm)
  // accepted range: 6rpm (may overheat) - 24rpm (may skip)
  // best range: 10rpm (safe, high torque) - 22rpm (fast, low torque)
  
  Serial.begin(9600); Serial.println();
  Serial.print("20 rpm = delay of ");
  Serial.print(stepper.getDelay()); // get delay between steps for set rpm
  Serial.print(" microseconds between steps");
  Serial.println();
  
  // void setTotalSteps(4076); // you can try this if you think your motor
                               // is geared 63.68395:1 rather than 64:1
                               // which would make the total steps 4076 (rather than default 4096)
                               // see: http://forum.arduino.cc/index.php?topic=71964.15 for more info
}

void loop() {
    moveClockwise = true;
    
    stepper.moveTo (moveClockwise, 2048);
    // move the stepper clockwise to position 2048 
    // which is 180 degrees, a half-turn (if using default of 4096 total steps)
    
    Serial.print("step position: ");  
    Serial.print(stepper.getStep()); // print the current step position to the console
    Serial.print(" / 4096");
    Serial.println();
    delay(1000); // wait a sec
    

    stepper.moveDegrees (moveClockwise, 90);
    // move the stepper 90 degrees clockwise, a quarter-turn
    // stepper.moveDegreesCW (90); <--- also works
    
    Serial.print("step position: ");
    Serial.print(stepper.getStep());
    Serial.print(" / 4096");
    Serial.println();
    delay(1000);
    

    moveClockwise = false; // let's change directions
   
    stepper.moveToDegree (moveClockwise, 0); 
    // moveClockwise is now false, so go counter-clockwise back to start (degree 0)
    
    Serial.print("step position: ");
    Serial.print(stepper.getStep());
    Serial.print(" / 4096");
    Serial.println();
    delay(1000);
    
}


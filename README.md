# CheapStepper v0.2 #
## An Arduino library for the 28BYJ-48 stepper motor  using ULN2003 driver board ##

#### created by Tyler Henry, 6/2016 ####


You can read some more info on the cheap yet worthy 28BYJ-48 stepper motor [here](https://arduino-info.wikispaces.com/SmallSteppers).

## Wiring example

<img src="https://github.com/tyhenry/CheapStepper/blob/master/extras/connections.png?raw=true" width="400">

## Library Info

### Half-stepping
CheapStepper uses an 8 mini-step sequence to perform all moves  
([a.k.a half-stepping](https://www.youtube.com/watch?v=B86nqDRskVU&feature=youtu.be&t=11m0s)): A-AB-B-BC-C-CD-D-DA

### Gear Ratio
Depending on whom you ask, the 28BYJ-48 motor has an internal gear ratio of either:  

- 64:1 (per manufacturer specs)  or  
- 63.68395:1 (measured... see this [Arduino Forum topic](http://forum.arduino.cc/index.php?topic=71964.15) for more info)

### Total Steps

64:1 gear ratio **\*** 64 steps (1 step = 8 mini-steps) per internal motor revolution =  
4096 total mini-steps / revolution  
or ~4076 (4075.7728) if the gear ratio is 63.68395:1  
  
CheapStepper library defaults to 4096 mini-steps but you can call:  
`CheapStepper::set4076StepMode()` to use 4076 steps
or `CheapStepper::setTotalSteps(int numSteps)` to use a custom amount


### Power
CheapStepper assumes a 5v power source for RPM calculations.  
It's best to use an external power supply like [this](https://www.adafruit.com/products/276), wired directly to the ULN2003 driver board, rather than draw from the Arduino's onboard power, which may have insufficient amperage (>100mA needed).

----
## Blocking Moves
_The Arduino sketch "pauses" during move()_

- move (boolean clockwise, int numSteps);
- moveTo (boolean clockwise, int toStep);
- moveDegrees (boolean clockwise, int degrees);
- moveToDegree (boolean clockwise, int toDegree);

----
## Non-blocking Moves 
_The Arduino sketch will continue running during the move.  
You must call run() on your stepper during loop()_  

- newMove (boolean clockwise, int numSteps);
- newMoveTo (boolean clockwise, int toStep);
- newMoveDegrees (boolean clockwise, int degrees);  
- newMoveToDegree (boolean clockwise, int toDegree);  

### Note
* must call run() during loop to continue move
* call stop() to cancel/end move

----
### Move a Single Mini-Step<br/>(1/8 of 8 Step Sequence)

- step (boolean clockwise);
- or stepCW(); or stepCCW();

----
#### All move functions have ...CW() or ...CCW() variants:  
e.g.  
 
- move 8 steps clockwise:  
  `move (true, 8);`  is the same as   
  `moveCW (8);` 
- create new move of 90 degrees counter-clockwise  
  `newMoveDegrees (false, 90);`  is the same as  
  `newMoveDegreesCCW (90);`

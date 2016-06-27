#CheapStepper v0.2#
##An Arduino library for the 28BYJ-48 stepper motor  using ULN2003 driver board##

####written by Tyler Henry, 6/2016####


[More info on the cheap but decent 28BYJ-48 stepper motor](https://arduino-info.wikispaces.com/SmallSteppers)

![wiring diagram](https://github.com/tyhenry/CheapStepper/blob/master/extras/connections.png?raw=true)

####library info:
uses 8-step sequence: A-AB-B-BC-C-CD-D-DA

motor has gear ratio of either:  
-  64:1 (per manufacturer specs)  or  
-  63.68395:1 measured (see: [Arduino Forum topic](http://forum.arduino.cc/index.php?topic=71964.15))  

64 * 64 steps per internal motor rev = 
4096 total mini-steps / revolution  
or ~4076 (4075.7728) depending on exact gear ratio

assumes 5v power source for rpm calc

####blocking moves
_arduino sketch pauses during move()_

- move (boolean clockwise, int numSteps);
- moveTo (boolean clockwise, int toStep);
- moveDegrees (boolean clockwise, int degrees);
- moveToDegree (boolean clockwise, int toDegree);

####non-blocking moves 
_arduino sketch will continue running during move  
must call run() on stepper during loop()_  

- newMove (boolean clockwise, int numSteps);
- newMoveTo (boolean clockwise, int toStep);
- newMoveDegrees (boolean clockwise, int degrees);  
- newMoveToDegree (boolean clockwise, int toDegree);  
  #####call run() during loop to continue move  
  #####call stop() to end move early


####move a single step (1/8 of 8 step sequence)

- step (boolean clockwise);
- or stepCW(); or stepCCW();

####All move functions have ...CW() or ...CCW() variants:  
e.g.  
 
- move (true, 8) == moveCW (8);  
  // move 8 steps clockwise  
- newMoveDegrees (false, 90) == newMoveDegreesCCW (90);  
  // set new move of 90 degrees counter-clockwise
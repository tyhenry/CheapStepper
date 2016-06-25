#CheapStepper v0.1#
##An Arduino library for the 28BYJ-48 stepper motor  using ULN2003 driver board##

####written by Tyler Henry, 6/2016####


[More info on the cheap but decent 28BYJ-48 stepper motor](https://arduino-info.wikispaces.com/SmallSteppers)

![wiring diagram](https://github.com/tyhenry/CheapStepper/blob/master/extras/connections.png?raw=true)

####library info:####
uses 8-step sequence: A-AB-B-BC-C-CD-D-DA

motor has gear ratio of either:  
-  64:1 (per manufacturer specs)  or  
-  63.68395:1 measured (see: [Arduino Forum topic](http://forum.arduino.cc/index.php?topic=71964.15))  

64 * 64 steps per internal motor rev = 
4096 total mini-steps / revolution  
or ~4076 (4075.7728) depending on exact gear ratio

assumes 5v power source for rpm calc

only includes blocking moves (i.e. will stop sketch during move) currently

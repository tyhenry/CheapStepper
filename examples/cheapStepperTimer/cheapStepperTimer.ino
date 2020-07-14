#include <uTimerLib.h>
#include <CheapStepper.h>

// pin #          ULN2003 pin
// 14      >--<   IN1
// 12      >--<   IN2
// 13      >--<   IN3
// 15      >--<   IN4

CheapStepper stepper(14,12,13,15);
int seq = 0;

void stepper_run() {
  stepper.run();
}

void setup() {
	stepper.setRpm(10);
	stepper.setSpr(4096);

	TimerLib.setInterval_us(stepper_run, stepper.interval());

	// let's just set up a serial connection and test print to the console
	Serial.begin(9600);
	Serial.println("Ready to start moving!");
}

void loop() {
	if (stepper.isReady()) switch(seq) {
		case 0:
			Serial.println("Move CW 90");
			stepper.moveDegreesCW(90);
			seq = 1;
			break;
		
		case 1:
			Serial.println("Move CCW 90");
			stepper.moveDegreesCCW(90);
			seq = 2;
			break;
		
		case 2:
			Serial.println("Move CW 360");
			stepper.moveDegreesCW(360);
			seq = 3;
			break;
		case 3:
			
			Serial.println("Stop");
			stepper.stop();
			seq = 4;
	}
}
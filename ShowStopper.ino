/* 
	ShowStopper By Andrea Campanella (emuboy85@gmail.com)_

	A Safety break system for AC motor:

	CoilA and CoilB are connected to two of the 3 phase AC motor, pumping DC into it, forcing it to stop.

	Input is debouced.

*/

const int SafeStop = 1;    		// the number of the pushbutton pin
const int CoilA = 0;      		// the number of the Coil A pin
const int CoilB = 2;     		// the number of the Coil B pin

int StopMotor = LOW;         	// the current state of the output pin
int StopMotorState;             // the current reading from the input pin
int lastStopMotorState = LOW;   // the previous reading from the input pin

long lastDebounceTime = 0;  	// the last time the output pin was toggled
long debounceDelay = 50;    	// the debounce time; increase if the output flickers , in milliseconds
int breake_duration = 10000; 	//how long the motor will breake.In seconds

void setup() {
  pinMode(SafeStop, INPUT);
  pinMode(CoilA, OUTPUT);
  pinMode(CoilB, OUTPUT);
  // set initial Coils state
  digitalWrite(CoilA, StopMotor);
  digitalWrite(CoilB, StopMotor);
}

void loop() {

  // Store the reading of the Stop in a variable
  int reading = digitalRead(SafeStop);

  // If the switch changed, due to noise or pressing:
  if (reading != lastStopMotorState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  } 
  
  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != StopMotorState) {
      StopMotorState = reading;

      // only toggle the LED if the new button state is HIGH
      if (StopMotorState == HIGH) {
			StopMotor = !StopMotor;
			// set the Coils:
			digitalWrite(CoilA, StopMotor);
			digitalWrite(CoilB, StopMotor);
			delay(breake_duration);
			digitalWrite(CoilA, !StopMotor);
			digitalWrite(CoilB, !StopMotor);
			
			// save the reading.  Next time through the loop,
			// it'll be the lastStopMotorState:
			lastStopMotorState = reading;
      }
    }
  }
}


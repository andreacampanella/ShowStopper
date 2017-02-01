/* 
  ShowStopper By Andrea Campanella (emuboy85@gmail.com)_

  A Safety break system for AC motor:

  EmergencyStopButton is the input from the user.
  InhibitUserControl disable the start/stop button and EmergencyStopButton , once the system is breaking the motor, nothing will stop it.
  DCInject is a relay output to 2 coils of the 3 phase motor, runnin DC in a AC motor this way will break it's run.
*/


const int EmergencyStopButton = 1;    // in
const int InhibitUserControl = 0;     // output
const int DCInject = 2;               // output

//to be chaged according the system
int EmergencyStopButtonState;                         // the current reading from the input pin
int LastEmergencyStopButtonState      = LOW;          // the previous reading from the input pin
int EmergencyStopButtonTriggeredState = HIGH          // the triggered state of the input pin 
int InhibitUserControlState           = HIGH;         // the active state of the output pin
int DCInjectState                     = HIGH;         // the active state of the output pin

// the following variables are long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.

long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 10;    // the debounce time; increase if the output flickers

void setup() {

  //set the pins direction
  pinMode(EmergencyStopButton, INPUT);
  pinMode(InhibitUserControl, OUTPUT);
  pinMode(DCInject, OUTPUT);

  // set initial outputs state as disable
  digitalWrite(InhibitUserControl, !InhibitUserControlState);
  digitalWrite(DCInject, !DCInjectState);

}

void loop() {
  // read the state of the switch into a local variable:
  int reading = digitalRead(EmergencyStopButton);

  // check to see if you just pressed the button 
  // (i.e. the input went from LOW to HIGH),  and you've waited 
  // long enough since the last press to ignore any noise:  

  // If the switch changed, due to noise or pressing:
  if (reading != LastEmergencyStopButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  } 
  
  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != EmergencyStopButtonState) {
      EmergencyStopButtonState = reading;

      // only toggle the LED if the new button state is HIGH
      if (EmergencyStopButtonState == EmergencyStopButtonTriggeredState) {
          
          digitalWrite(InhibitUserControl, InhibitUserControlState);    //disable the emergency button 
          delay(250);                                                   //wait for the system to react
          digitalWrite(DCInject, DCInjectState);                        //inject DC in to motor
          delay(9750);                                                  //and keep doing it for ~9 secs
          digitalWrite(DCInject, !DCInjectState);                       //release the motor
          digitalWrite(InhibitUserControl, !InhibitUserControlState);   //reenable user controls 
      }
    }
  }
  // save the reading.  Next time through the loop,
  // it'll be the LastEmergencyStopButtonState:
  LastEmergencyStopButtonState = reading;
}


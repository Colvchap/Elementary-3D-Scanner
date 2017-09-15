/* Sweep
 by BARRAGAN <http://barraganstudio.com>
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Sweep
*/

#include <Servo.h>

Servo panservo;  // create servo object for panning
Servo tiltservo; // create servo object for tilting
// twelve servo objects can be created on most boards

// Set constants
const int INTERVAL = 500; //set button interval
const int TILTMIN = 30;   //Set min tilt angle
const int TILTMAX = 150;  //Set max tilt angle
const int PANMIN = 30;    //Set min pan angle
const int PANMAX = 150;   //Set max pan angle
const int STEP = 1;       //Set step for angle

//Set up pins
const int startPin = 3;
const int modePin = 2;
const int panServoPin = 8;
const int tiltServoPin = 9;
const int analogInPin = 0; //Set the analogInPin
const int analogOutPin = 7; // Analog output pin that the LED is attached to


// variables
int pan = 0;    // variable to store the pan servo position
int tilt = 0;   //Store the tilt servo position
int AnalogInPin = 0;                 // analog pin used to connect the IR sensor
int val = 0;                 // variable to store the values from sensor(initially zero)
int buttonState = 0;
int lastButtonState = 0;
unsigned long previousMillisButton = 0;       // store last time the button was checked to see if it was being pressed
const int delayInterval = 300; //Set the delay time between button checks
int count = 0;
int buttonMode = 0;


void setup() {
  pinMode(startPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(startPin), buttonUpdate, CHANGE);

  pinMode(modePin, INPUT);
  attachInterrupt(digitalPinToInterrupt(startPin), buttonUpdate, CHANGE);

  pinMode(analogInPin, INPUT_PULLUP);
  pinMode(analogOutPin, OUTPUT);

  panservo.attach(panServoPin);  // attaches the pan servo on pin 8 to the servo object
  tiltservo.attach(tiltServoPin); // attaches the tilt servo on pin 9 to the servo object

  Serial.begin(9600);               // starts the serial monitor
  Serial.println("Val \ttilt \tpan");
}

void loop() {
  if ( count = 0) {
    panservo.write((PANMIN + PANMAX)/2);
    tiltservo.write((TILTMIN + TILTMAX/2));
    count += 1;
  }
  interrupts();
  buttonState = digitalRead(startPin);
  delay(5);
}

void readIn()
{
  val = analogRead(AnalogInPin);       // reads the value of the sharp sensor
  Serial.print(val);            // prints the value of the sensor to the serial monitor
  Serial.print("\t");
  Serial.print(tilt);
  Serial.print("\t");
  Serial.print(pan);
  Serial.println("");
}

void fullScan()
{
  interrupts();
  panservo.write(PANMIN);
  tiltservo.write(TILTMIN);
  for (tilt = TILTMIN; tilt <= TILTMAX; tilt += STEP) { // goes from 0 degrees to 180 degrees
    for (pan = PANMIN; pan <= PANMAX; pan += STEP) { // goes from 180 degrees to 0 degrees
      panservo.write(pan);              // tell servo to go to position in variable 'pos'
      readIn();
      delay(40);                       // waits 15ms for the servo to reach the position
      Serial.println(count);
      if (count >= 4){
        Serial.println("Returned");
        return;
      }
    }
    tiltservo.write(tilt);              // tell servo to go to position in variable 'pos'
    panservo.write(PANMIN);
    readIn();
    delay(30);                       // waits 15ms for the servo to reach the position
  }
  
  
}

void buttonUpdate() {
  unsigned long currentMillis = millis();
  buttonState = digitalRead(startPin);
  if ((currentMillis - previousMillisButton) >= delayInterval){
    if (buttonState == HIGH && lastButtonState != HIGH) {
          fullScan();
        } 
    if (buttonState == HIGH) {
      Serial.println("ButtonSTate HIGH");
      Serial.print(count);
      count += 1;
    }
    else {
      count = 0;
    }
    previousMillisButton = currentMillis;    
    lastButtonState = buttonState;
  }
}


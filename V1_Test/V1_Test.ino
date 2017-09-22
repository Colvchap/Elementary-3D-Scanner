/* Sweep
 by BARRAGAN <http://barraganstudio.com>
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Sweep
*/

#include <Servo.h>
#include <math.h>

Servo panservo;  // create servo object for panning
Servo tiltservo; // create servo object for tilting
// twelve servo objects can be created on most boards

// Set constants
const int INTERVAL = 500; //set button interval
const int TILTMIN = 45;   //Set min tilt angle
const int TILTMAX = 135;  //Set max tilt angle
const int PANMIN = 45;    //Set min pan angle
const int PANMAX = 135;   //Set max pan angle
const int STEP = 1;       //Set step for angle
const float Pi = 3.14159;

//Set up pins
const int startPin = 3;
const int modePin = 2;
const int panServoPin = 8;
const int tiltServoPin = 9;
const int analogInPin = 0; //Set the analogInPin
const int analogOutPin = 7; // Analog output pin that the LED is attached to
const int ledPin = 10;
const int distance = 50;
const int b = 6.5048;
const int m = -0.0185;


// variables
int pan = 0;    // variable to store the pan servo position
int tilt = 0;   //Store the tilt servo position
int AnalogInPin = 0;                 // analog pin used to connect the IR sensor
int val = 0;                 // variable to store the values from sensor(initially zero)
int buttonState = 0;
int lastButtonState = 0;
int lastButtonState2 = 0;
unsigned long previousMillisButton = 0;       // store last time the button was checked to see if it was being pressed
unsigned long previousMillisButton2 = 0;
const int delayInterval = 200; //Set the delay time between button checks
int count = 0;
int buttonMode = 0;
int ledState = HIGH;
int corrval = 0;


void setup() {
  pinMode(startPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(startPin), buttonUpdate, CHANGE);

  pinMode(modePin, INPUT);
  attachInterrupt(digitalPinToInterrupt(modePin), buttonModeChange, CHANGE);

  pinMode(analogInPin, INPUT_PULLUP);
  pinMode(analogOutPin, OUTPUT);
  pinMode(ledPin, OUTPUT);

  panservo.attach(panServoPin);  // attaches the pan servo on pin 8 to the servo object
  tiltservo.attach(tiltServoPin); // attaches the tilt servo on pin 9 to the servo object

  Serial.begin(9600);               // starts the serial monitor
  Serial.println("Val \tcorval \ttilt \tpan \txval \tyval \tzval");
  digitalWrite(ledPin, ledState);
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
  double cortilt = (tilt-90)/180*2*Pi;
  double corpan = (-90)/180*2*Pi;
  // double corrval = sqrt(distance^2 + (tan(cortilt)*distance)^2 + (tan(corpan)*distance)^2);
  double cordist = (log(val)-b)/m);
  int z = (int)(cordist*sin(cortilt);
  double roe = cordist * cos(cortilt);
  int x = roe*cos(corpan);
  int y = roe*sin(corpan);
  Serial.print(val);            // prints the value of the sensor to the serial monitor
  Serial.print("\t");
  Serial.print(cordist);            // prints the value of the sensor to the serial monitor
  Serial.print("\t");
  Serial.print(tilt);
  Serial.print("\t");
  Serial.print(xval);
  Serial.print("\t");
  Serial.print("xval");
  Serial.print(yval);
  Serial.print("\t");
  Serial.print("yval");
  Serial.print(zval);
  Serial.print("\t");
  Serial.print("zval");
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
      delay(5);                       // waits 15ms for the servo to reach the position
      Serial.println(count);
      if (buttonMode == 0){
        break;
      }
    }
    tiltservo.write(tilt);              // tell servo to go to position in variable 'pos'
    panservo.write(PANMIN);
    readIn();
    delay(5); // waits 15ms for the servo to reach the position
    if (buttonMode == 0){
        break;
      }
  }
  count = 0; 
}

void halfScan()
{
  interrupts();
  panservo.write(PANMIN);
  for (pan = PANMIN; pan <= PANMAX; pan += STEP) { // goes from 180 degrees to 0 degrees
    panservo.write(pan);              // tell servo to go to position in variable 'pos'
    readIn();
    delay(40);                       // waits 15ms for the servo to reach the position
    if (buttonMode == 0){
      break;
    }
  }
  panservo.write(PANMIN);
  count = 0; 
}

void buttonUpdate() 
{
  unsigned long currentMillis = millis();
  buttonState = digitalRead(startPin);
  if (buttonMode == 1)
  {
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
  else if (buttonMode == 2){
    if ((currentMillis - previousMillisButton) >= delayInterval){
    if (buttonState == HIGH && lastButtonState != HIGH) {
          halfScan();
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
  else {
    Serial.println("Test time!");
    for (int x = 0; x < 5; x ++){
      readIn();
      delay(100);
    }
  }
}

void buttonModeChange() {
  unsigned long currentMillis = millis();
  buttonState = digitalRead(modePin);
  if ((currentMillis - previousMillisButton2) >= delayInterval){
    if (buttonState == HIGH && lastButtonState2 != HIGH) {
      ledState = !ledState;
      digitalWrite(ledPin, ledState);
      buttonMode = (++buttonMode) % 3;
      count = 0;
      Serial.println("WE CHANGED! to: ");
      Serial.println(buttonMode);
    }
    lastButtonState2 = buttonState;
    previousMillisButton2 = currentMillis;
  }
}


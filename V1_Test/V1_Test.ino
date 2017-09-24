//Headers to include:
#include <Servo.h>
#include <math.h>

Servo panservo;  // create servo object for panning
Servo tiltservo; // create servo object for tilting
// twelve servo objects can be created on most boards

// Set constants
const int INTERVAL = 500;   //set button interval
const int TILTMIN = 45;     //Set min tilt angle
const int TILTMAX = 135;    //Set max tilt angle
const int PANMIN = 45;      //Set min pan angle
const int PANMAX = 135;     //Set max pan angle
const int STEP = 1;         //Set step for angle
const double Pi = 3.14159;  //Pi.
const double b = 6.5048;    //The constant b from the calibration done in MATLAB.
const double m = -0.0185;   //The constant m from the calibration done in MATLAB

//Set up pins
const int startPin = 3;     //The pin of the button that starts the test
const int modePin = 2;      //The pin of the button that changes modes
const int panServoPin = 8;  //The pin of the pan servo
const int tiltServoPin = 9; //The pin of the tilt servo
const int analogInPin = 0;  //Set the pin that the IR sensor will print to
const int ledPin = 10;      //The pin of the LED



// variables
int pan = 0;                  // Stores the pan servo position
int tilt = 0;                 // Stores the tilt servo position
int val = 0;                  // Stores the values from sensor
int buttonState = 0;          // Stores the current button state
int lastButtonState = 0;      // Stores the last button state for the first button
int lastButtonState2 = 0;     // Stores the last button state for the second button
unsigned long previousMillisButton = 0;       // store last time the button was checked to see if it was being pressed
unsigned long previousMillisButton2 = 0;      // Stores last time second button was checked
const int delayInterval = 200; //Set the delay time between button checks
int buttonMode = 0;           //Used to store which mode the project is in.
int ledState = HIGH;          //Used to change the LED state if it isn't in testing mode.
int corrval = 0;              //The corrected val value.


void setup() {
  //Attach the start pin and allow it to interrupt
  pinMode(startPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(startPin), buttonUpdate, CHANGE);

  //Attach the mode changing pin, and allow it to interrupt.
  pinMode(modePin, INPUT);
  attachInterrupt(digitalPinToInterrupt(modePin), buttonModeChange, CHANGE);

  //Attach misc. pins
  pinMode(analogInPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);

  //Attach servos
  panservo.attach(panServoPin);  // attaches the pan servo on pin 8 to the servo object
  tiltservo.attach(tiltServoPin); // attaches the tilt servo on pin 9 to the servo object

  Serial.begin(9600);               // starts the serial monitor
  Serial.println("Val \tcorval \ttilt \tpan \txval \tyval \tzval"); //Prints what the information will be in each column.
  digitalWrite(ledPin, ledState);   //Write the starting LED state.
}

void loop() {
  interrupts();   //allow interrupts
  delay(10);
}

void readIn()
{
  //Read in from the IR sensor, and print the results to serial.
  
  val = analogRead(analogInPin);       // reads the value of the sharp sensor

  //Gets the corrected values for what will be printed
  double cortilt = (tilt-90.0)/180.0*2.0*Pi;
  double corpan = (pan-90.0)/180.0*2.0*Pi;
  double cordist = (log(val)-b)/m;
  int zval = (cordist*sin(cortilt));
  double roe = cordist * cos(cortilt);
  int xval = (roe*cos(corpan));
  int yval = (roe*sin(corpan));

  //Print the data obtained from the read.
  Serial.print(val);
  Serial.print("\t");
  Serial.print(cordist);
  Serial.print("\t");
  Serial.print(tilt);
  Serial.print("\t");
  Serial.print(pan);
  Serial.print("\t");
  Serial.print(xval);
  Serial.print("\t");
  Serial.print(yval);
  Serial.print("\t");
  Serial.print(zval);
  Serial.println("");
}

void fullScan()
{
  //Do a "full" scan of an object (use both the pan and tilt functions).

  //allow interrupts (to reset)
  interrupts();

  //Set the servos to start at their minimum values
  panservo.write(PANMIN);
  tiltservo.write(TILTMIN);

  //Go from the minimums to maximums for hte two servos, pause, then take a reading.
  for (tilt = TILTMIN; tilt <= TILTMAX; tilt += STEP) {
    for (pan = PANMIN; pan <= PANMAX; pan += STEP) { 
      panservo.write(pan);
      delay(50); 
      for (int x = 1; x <= 5; x++){
        readIn(); 
        delay(5);
      }
      //If the button mode changes, break out of this loop.
      if (buttonMode != 1){
        break;
      }
    }
    //Increment tilt and go back to the pan minimum.
    tiltservo.write(tilt);
    panservo.write(PANMIN);

    //Make sure there's enough time for the servos to move to the position
    delay(500); 

    
    if (buttonMode == 0){
        break;
      }
  }
  
  //Go back to the starting position of scanning
  panservo.write((PANMIN + PANMAX)/2);
  tiltservo.write((TILTMIN + TILTMAX/2));
}

void halfScan()
{
  //Do a "half scan," or just a horizontal scan, of an object.
  interrupts();
  panservo.write(PANMIN);
  for (pan = PANMIN; pan <= PANMAX; pan += STEP) {
    panservo.write(pan);              
    readIn();
    delay(40);                      
    if (buttonMode == 0){
      break;
    }
  }
  panservo.write(PANMIN);
}

void buttonUpdate() 
{
  //The function that activates the correct sequence of events, depending on what the button mode is.
  unsigned long currentMillis = millis();
  buttonState = digitalRead(startPin);
  //Option 1: Do a full scan
  if (buttonMode == 1)
  {
  if ((currentMillis - previousMillisButton) >= delayInterval){
    if (buttonState == HIGH && lastButtonState != HIGH) {
          fullScan();
        } 
    previousMillisButton = currentMillis;    
    lastButtonState = buttonState;
  }
  }
  //Option 2: Do a half scan
  else if (buttonMode == 2){
    if ((currentMillis - previousMillisButton) >= delayInterval){
    if (buttonState == HIGH && lastButtonState != HIGH) {
          halfScan();
        } 
    previousMillisButton = currentMillis;    
    lastButtonState = buttonState;
    }
  }

  //Option 3: ping an object (for calibration)
  else {
    if ((currentMillis - previousMillisButton) >= delayInterval){
      Serial.println("Test time!");
      for (int x = 0; x < 5; x ++){
        readIn();
        delay(100);
      }
    }
  previousMillisButton = currentMillis;
  lastButtonState = buttonState;
  }
}

void buttonModeChange() {
  //Change what mode the device is in.
  unsigned long currentMillis = millis();
  buttonState = digitalRead(modePin);
  if ((currentMillis - previousMillisButton2) >= delayInterval){
    if (buttonState == HIGH && lastButtonState2 != HIGH) {
      ledState = !ledState;
      digitalWrite(ledPin, ledState);
      buttonMode = (++buttonMode) % 3;
      Serial.println("WE CHANGED! to: ");
      Serial.println(buttonMode);
    }
    lastButtonState2 = buttonState;
    previousMillisButton2 = currentMillis;
  }
}


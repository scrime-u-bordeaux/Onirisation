#include <Servo.h>

/*  
 *  modify the folowing variables to fit the number of fans, servos, sensors 
 */

const int panelPin = 11;                 // led panel pin
const int servoPins[] = {2, 4, 7};       // non PWM pins for servos  
const int fsrAnalogPin[] = {0};          // FSR analog pins
const int ventilPin[] = {3, 5, 6};       // PWM pins for ventilos
const int lowTresh = 30;                 // set lowest limit for driving ventilos
const int highTresh = 170;               // set highest limit for driving ventilos
const unsigned long lagTime = 1000;      // set lag time for driving each ventilos
const int noDialPin = 9;
const int countPin = 10;

/*  ventilVal
 *  deffault parameters 
 */

const int servoNum = sizeof(servoPins) / sizeof(servoPins[0]);        // number of servos
const int fsrNum = sizeof(fsrAnalogPin) / sizeof(fsrAnalogPin[0]);    // number of resistive sensors
const int ventilNum = sizeof(ventilPin) / sizeof(ventilPin[0]);       // number of ventilos

Servo myServos[servoNum];                   // create servo objects
int fsrReading[fsrNum];                     // value sent to score
int ventilVal[ventilNum];                   // value received from score
int previousVal[ventilNum];                 // keep track of previous vlaues
unsigned long timeElapsed;                  // keep track of time past
unsigned long lastTime[ventilNum];          // store date for mesuring durations
int count = 0;
int prevState = 0; 

void setup() {
  
  Serial.begin(9600);                       // set baud rate
  
  for (int i = 0; i < ventilNum; i++) {
    pinMode(ventilPin[i], OUTPUT);         // define pin as output
    previousVal[i] = 0;                    // initialise all previousVal to 0
    lastTime[i] = 0;                       // initialise all lastTimes to 0
    ventilVal[i] = 0;                      // initialise all ventilVal to 0
  }
  
  for (int i = 0; i < servoNum; i++) {
    myServos[i].attach(servoPins[i], 800, 2200);   // define servo range
    myServos[i].writeMicroseconds(1500);            // servo initial value
  }
      
  pinMode(panelPin, OUTPUT);         // define pin as output
  pinMode(noDialPin, INPUT_PULLUP);    // define pin as inpout
  pinMode(countPin, INPUT_PULLUP);    // define pin as inpout

}

void loop() {
  
  timeElapsed = millis();                   // update current date
  /*
  for (int i = 0; i < fsrNum; i++) {
    fsrReading[i] = analogRead(fsrAnalogPin[i]);
    Serial.print(fsrReading[i]);            // read anog pin and send it through serial
    if (i == (fsrNum - 1)) {
      Serial.println();
    } else {
      Serial.print(',');
    }
  }
 */

 if(!digitalRead(noDialPin)) {
  int curentState = digitalRead(countPin);
  if((curentState == 0) && (prevState == 1)) {
    count++;
  }
  prevState = curentState;
 } else {
  if(count != 0) {
    Serial.println(count - 1);
    count = 0;
  } 
 }

// Serial.print(digitalRead(dialPin)) ; 
  
  while (Serial.available() > 0) {    
    switch (Serial.read()) {                // switch for the folowing characters 
      case 'p': 
      analogWrite(panelPin, Serial.parseInt());
      break;  
      case 'l':
      myServos[0].writeMicroseconds(
        map(Serial.parseInt(), 0, 90, 800, 2200) 
        );
      break;
      case 'c':
      myServos[1].writeMicroseconds(
        map(Serial.parseInt(), 0, 90, 800, 2200) 
        );
      break;
      case 'r':
      myServos[2].writeMicroseconds(
        map(Serial.parseInt(), 0, 90, 800, 2200) 
        );
      break;
      case 'v': 
      ventilVal[0] = Serial.parseInt();
      break;
      case 'w': 
      ventilVal[1] = Serial.parseInt();
      break;
      case 'x': 
      ventilVal[2] = Serial.parseInt();
      break;
    }
  }
  
  for (int i = 0; i < ventilNum; i++) { 
    if ((timeElapsed - lastTime[i]) >= lagTime) {
      if (needStarter(ventilVal[i], previousVal[i])) {
        analogWrite(ventilPin[i], 255);
        lastTime[i] = timeElapsed;
        } else {
        analogWrite(ventilPin[i], ventilVal[i]);
      }
    }
  }
  
  Serial.flush();
  delay(1);
}

bool needStarter(int val, int &prev) {
  
  if ((val >= lowTresh) && (val < highTresh)) {
    if (prev < highTresh) {
      prev = 255;
      return true;
    } else {
      return false;
    }
  } else {
    prev = val;
    return false;
  }
}

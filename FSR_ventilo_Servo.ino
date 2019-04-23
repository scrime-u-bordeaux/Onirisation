#include <Servo.h>

/*  
 *  modify the folowing variables to fit the number of fans, servos, sensors 
 */

const int servoNum = 2;                     // number of servos

const int fsrNum = 1;                       // number of resistive sensors
const int fsrAnalogPin[] = {0};             // FSR analog pins

const int ventilNum = 1;                    // number of ventilos
const int ventilPin[] = {11};               // connect ventilos to PWM pins
int previousVal[] = {0};                    // initialise all previousVal to 0
const int lowVentil[] = {50};               // set lowest limit for driving each ventilos
const int highVentil[] = {170};             // set lowest limit for driving each ventilos
const unsigned long ventilLag[] = {1000};   // set lag time for driving each ventilos
unsigned long lastTime[] = {0};             // store date for mesuring durations

Servo myServos[servoNum] ;                  // create servo object to con 
int fsrReading[fsrNum];                     // value sent to score
int ventilVal[ventilNum];                   // value received from score
unsigned long timeElapsed;

void setup() {
  
  Serial.begin(9600);                       // set baud rate
  
  for (int i = 0; i < ventilNum; i++)
    pinMode(ventilPin[i], OUTPUT);          // define pin as output
  
  for (int i = 0; i < servoNum; i++) {
    myServos[i].attach(7  i, 800, 2200);   // define servo range
    myServos[i].write(90);                  // servo initial value
  }
}

void loop() {
  
  timeElapsed = millis();                   // update current date
  
  for (int i = 0; i < fsrNum; i++) {
    fsrReading[i] = analogRead(fsrAnalogPin[i]);
    Serial.print(fsrReading[i]);            // read anog pin and send it through serial
    if (i == (fsrNum - 1)) {
      Serial.println();
    } else {
      Serial.print(',');
    }
  }
  
  while (Serial.available() > 0) {    
    switch (Serial.read()) {                // switch for the folowing characters 
      case 'l':
      myServos[0].writeMicroseconds(
        map(Serial.parseInt(), 0, 150, 800, 2200) 
        );
      case 'r':
      myServos[1].writeMicroseconds(
        map(Serial.parseInt(), 0, 150, 800, 2200) 
        );
      break;
      case 'v': 
      ventilVal[0] = Serial.parseInt();
      break;
    }
  }
  
  for (int i = 0; i < ventilNum; i++) { 
    if ((timeElapsed - lastTime[i]) >= ventilLag[i]) {
      if (needStarter(ventilVal[i], previousVal[i], lowVentil[i], highVentil[i])) {
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

bool needStarter(int val, int &prev, int lowTresh , int highTresh) {
  
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

/*
 *  Control Servo Through Potentiometer.
 *
 *  Copyright (C) 2010 Efstathios Chatzikyriakidis (stathis.chatzikyriakidis@gmail.com)
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

// include library for servo.
#include <Servo.h>

Servo myServo; // create servo object to control a servo.

const int servoPin = 9;  // the pin number (PWM) for the servo.
const int sensorPin = 0; // the pin number for the input sensor.

const int ledPinA = 11; // one led (PWM) for left/right position.
const int ledPinB = 6;  // one led (PWM) for left/right position.
const int ledPinC = 13; // calibration status led.

// the following two bounds are important for the sensor calibration.
int sensorMin = 1023;
int sensorMax = 0;

// calibration time in millis.
const int CAL_TIME = 10000;

// default value for servo and leds' devices.
int valServo = 0;
int valLeds = 0;

// startup point entry (runs once).
void setup() {
  // set leds as output.
  pinMode(ledPinA, OUTPUT);
  pinMode(ledPinB, OUTPUT);
  pinMode(ledPinC, OUTPUT);

  // set sensor as input.
  pinMode(sensorPin, INPUT);

  // perform the calibration.
  calibrationPeriod();

  // attach the servo on a pin.
  myServo.attach(servoPin);
} 

// loop the main sketch.
void loop() {
  // get the value from the sensor.
  valServo = (valLeds = analogRead(sensorPin)); 

  // map the value for the leds (PWM).
  valLeds = map(valLeds, 0, 1023, 0, 255);

  // opposite-oriented leds (left/right).
  analogWrite(ledPinA, 255-valLeds);
  analogWrite(ledPinB, valLeds);

  // map the value for the servo.
  valServo = map(valServo, 0, 1023, 0, 179);

  // move the servo.
  myServo.write(valServo);

  // wait some time the servo to move.
  delay(15);
}

// input sensor calibration process.
void calibrationPeriod () {
  // the value that input sensor sends.
  int sensorValue = 0;

  // signal the start of the calibration period.
  digitalWrite(ledPinC, HIGH);

  // calibrate during the some time.
  while (millis() < CAL_TIME) {
    sensorValue = analogRead(sensorPin);

    // record the maximum sensor value.
    if (sensorValue > sensorMax) {
      sensorMax = sensorValue;
    }

    // record the minimum sensor value.
    if (sensorValue < sensorMin) {
      sensorMin = sensorValue;
    }
  }

  // signal the end of the calibration period.
  digitalWrite(ledPinC, LOW);
}

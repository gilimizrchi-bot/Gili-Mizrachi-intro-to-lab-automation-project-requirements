/*
  Project 11 - Fan control with Arduino SensorKit accelerometer
  - Servo signal on D3
  - Fan power enable on D7
  - Read accelerometer X from Arduino SensorKit
  - Move servo 0..170 based on X-axis
  - Turn fan on when X axis is between 45 and 105
*/

#include <Servo.h>
#include "Arduino_SensorKit.h"

const int SERVO_PIN = 3;
const int FAN_POWER_PIN = 7;
const int SERVO_MIN_ANGLE = 0;
const int SERVO_MAX_ANGLE = 170;
const int FAN_ON_ACCEL_MIN = 45;
const int FAN_ON_ACCEL_MAX = 105;
const unsigned long LOOP_INTERVAL_MS = 100;

Servo fanServo;
unsigned long lastLoopTime = 0;

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ;
  }

  pinMode(FAN_POWER_PIN, OUTPUT);
  digitalWrite(FAN_POWER_PIN, LOW);

  fanServo.attach(SERVO_PIN);
  setServoAngle(SERVO_MIN_ANGLE);

  Serial.println("Project 11 - SensorKit fan control started");
  Accelerometer.begin();
  delay(100);
  Serial.println("Accelerometer initialized");
}

void loop() {
  unsigned long now = millis();
  if (now - lastLoopTime < LOOP_INTERVAL_MS) {
    return;
  }
  lastLoopTime = now;

  int rawX = Accelerometer.readX();
  int rawY = Accelerometer.readY();
  int rawZ = Accelerometer.readZ();

  // The servo angle is driven by the X-axis reading.
  // If rawX changes when you tilt the sensor along its X axis, the servo will move.
  int maxRaw = (rawX > 255 ? 1023 : 255);
  int servoAngle = map(constrain(rawX, 0, maxRaw), 0, maxRaw, SERVO_MIN_ANGLE, SERVO_MAX_ANGLE);
  setServoAngle(servoAngle);

  bool fanOn = (rawX >= FAN_ON_ACCEL_MIN && rawX <= FAN_ON_ACCEL_MAX);
  digitalWrite(FAN_POWER_PIN, fanOn ? HIGH : LOW);

  Serial.print("rawX=");
  Serial.print(rawX);
  Serial.print(" rawY=");
  Serial.print(rawY);
  Serial.print(" rawZ=");
  Serial.print(rawZ);
  Serial.print(" | servo=");
  Serial.print(servoAngle);
  Serial.print(" | fan=");
  Serial.println(fanOn ? "ON" : "OFF");
}

void setServoAngle(int angle) {
  angle = constrain(angle, SERVO_MIN_ANGLE, SERVO_MAX_ANGLE);
  fanServo.write(angle);
}

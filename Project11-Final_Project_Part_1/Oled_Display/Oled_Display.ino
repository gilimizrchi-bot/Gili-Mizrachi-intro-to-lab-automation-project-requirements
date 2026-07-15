#include "Arduino_SensorKit.h"
#include <Servo.h>

const int FAN_POWER_PIN = 7;
const int SERVO_PIN = 3;
const int BUZZER_PIN = 5;
const int ANGLE_THRESHOLD = 45;
const int SERVO_MIN_ANGLE = 0;
const int SERVO_MAX_ANGLE = 170;
const int FAN_ACTIVE_MIN_ANGLE = 46;

Servo fanServo;
int currentServoAngle = 90;

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ;
  }

  Oled.begin();
  Oled.setFlipMode(true);

  pinMode(FAN_POWER_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  fanServo.attach(SERVO_PIN);
  setServoAngle(90);
  Accelerometer.begin();

  noTone(BUZZER_PIN);
}

void loop() {
  updateServoFromAccelerometer();
  bool fanIsWorking = (currentServoAngle >= FAN_ACTIVE_MIN_ANGLE);
  bool buzzerOn = (currentServoAngle <= ANGLE_THRESHOLD);

  if (buzzerOn) {
    tone(BUZZER_PIN, 2000);
  } else {
    noTone(BUZZER_PIN);
  }

  setFanPower(fanIsWorking);
  displayStatus(currentServoAngle, buzzerOn);
  delay(200);
}

void setFanPower(bool enabled) {
  digitalWrite(FAN_POWER_PIN, enabled ? HIGH : LOW);
}

void setServoAngle(int angle) {
  currentServoAngle = constrain(angle, SERVO_MIN_ANGLE, SERVO_MAX_ANGLE);
  fanServo.write(currentServoAngle);
}

void updateServoFromAccelerometer() {
  float xValue = Accelerometer.readX();
  float ratio = constrain(xValue, -1.0f, 1.0f);
  int mappedAngle = (int)((ratio + 1.0f) * 85.0f);
  setServoAngle(mappedAngle);
}

void displayStatus(int angle, bool buzzerOn) {
  Oled.setFont(u8x8_font_chroma48medium8_r);

  Oled.setCursor(0, 0);
  Oled.print("Angle: ");
  Oled.print(angle);

  Oled.setCursor(0, 3);
  Oled.print("Buzzer: ");
  Oled.print(buzzerOn ? "ON" : "OFF");
}

#include <SPI.h>
#include <SD.h>
#include "Arduino_SensorKit.h"
#include <Servo.h>

const int FAN_POWER_PIN = 7;
const int SERVO_PIN = 3;
const int BUZZER_PIN = 5;
const int RECORD_BUTTON_PIN = 2;
const int ANGLE_THRESHOLD = 45;
const int SERVO_MIN_ANGLE = 0;
const int SERVO_MAX_ANGLE = 170;
const int FAN_ACTIVE_MIN_ANGLE = 46;
const unsigned long LOG_INTERVAL_MS = 500;
const int SD_CS_PIN = 10;

Servo fanServo;
int currentServoAngle = 90;
unsigned long lastLogTime = 0;
bool sessionRunning = false;
bool lastButtonState = HIGH;
File dataFile;

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ;
  }

  Oled.begin();
  Oled.setFlipMode(true);

  pinMode(FAN_POWER_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(RECORD_BUTTON_PIN, INPUT_PULLUP);

  fanServo.attach(SERVO_PIN);
  setServoAngle(90);
  Accelerometer.begin();

  if (!SD.begin(SD_CS_PIN)) {
    Serial.println("SD card init failed");
    while (true) {
      delay(1000);
    }
  }

  if (SD.exists("session_data.csv")) {
    SD.remove("session_data.csv");
  }

  dataFile = SD.open("session_data.csv", FILE_WRITE);
  if (dataFile) {
    dataFile.println("time_ms,angle,buzzer_state");
    dataFile.flush();
  }

  noTone(BUZZER_PIN);
}

void loop() {
  bool buttonState = digitalRead(RECORD_BUTTON_PIN);

  if (lastButtonState == HIGH && buttonState == LOW) {
    sessionRunning = !sessionRunning;
    if (sessionRunning) {
      lastLogTime = millis();
      Serial.println("START");
    } else {
      Serial.println("STOP");
      noTone(BUZZER_PIN);
      setFanPower(false);
      displayStatus(currentServoAngle, false);
    }
    delay(300);
  }
  lastButtonState = buttonState;

  if (!sessionRunning) {
    noTone(BUZZER_PIN);
    setFanPower(false);
    displayStatus(currentServoAngle, false);
    delay(100);
    return;
  }

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

  if (millis() - lastLogTime >= LOG_INTERVAL_MS) {
    lastLogTime = millis();
    logDataToSD(buzzerOn);
  }

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

void logDataToSD(bool buzzerState) {
  if (!dataFile) {
    return;
  }

  dataFile.print(millis());
  dataFile.print(",");
  dataFile.print(currentServoAngle);
  dataFile.print(",");
  dataFile.println(buzzerState ? "ON" : "OFF");
  dataFile.flush();
}

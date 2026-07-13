const int rotaryPin = A0;
const int ledPin = 3;

void setup()
{
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
}

void loop()
{
  static int lastRotaryValue = -1;

  int rotaryValue = analogRead(rotaryPin);

  if (rotaryValue != lastRotaryValue)
  {
    int pwmValue = map(rotaryValue, 0, 1023, 0, 255);
    analogWrite(ledPin, pwmValue);

    Serial.print("Rotary: ");
    Serial.print(rotaryValue);
    Serial.print(" -> PWM: ");
    Serial.println(pwmValue);

    lastRotaryValue = rotaryValue;
  }

  delay(50);
}


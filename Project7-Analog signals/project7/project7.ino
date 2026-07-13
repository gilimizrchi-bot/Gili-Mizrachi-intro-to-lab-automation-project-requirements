const int rotaryPin = A0;

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  int rotaryValue = analogRead(rotaryPin);
  Serial.println(rotaryValue);
  delay(200);
}


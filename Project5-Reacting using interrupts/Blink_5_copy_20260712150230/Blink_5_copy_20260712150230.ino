const int buttonPin = 6;
const int ledPin = 4;

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);  // button press reads LOW with the internal pull-up
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);        // LED is off by default

  Serial.begin(9600);
  Serial.println("Button LED demo started");
}

void loop() {
  int buttonState = digitalRead(buttonPin);

  if (buttonState == HIGH) {
    digitalWrite(ledPin, HIGH);
    Serial.println("Button pressed -> LED ON");
  } else {
    digitalWrite(ledPin, LOW);
    Serial.println("Button released -> LED OFF");
  }

  delay(50);
}

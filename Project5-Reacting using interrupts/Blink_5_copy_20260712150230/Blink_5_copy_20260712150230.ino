const int interruptPin = 2;  // Only pins 2 and 3 support external interrupts on Arduino Uno/Nano
const int ledPin = 4;
volatile bool ledOn = false;
volatile bool risingEdgeDetected = false;
volatile bool fallingEdgeDetected = false;
unsigned long onStartTime = 0;
unsigned long onDuration = 0;

void buttonISR() {
  if (digitalRead(interruptPin) == HIGH) {
    risingEdgeDetected = true;
  } else {
    fallingEdgeDetected = true;
  }
}

void setup() {
  pinMode(interruptPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  attachInterrupt(digitalPinToInterrupt(interruptPin), buttonISR, CHANGE);

  Serial.begin(9600);
  Serial.println("Interrupt-based button LED demo started");
}

void loop() {
  if (risingEdgeDetected) {
    ledOn = true;
    onStartTime = millis();
    digitalWrite(ledPin, HIGH);
    Serial.println("Rising edge detected -> LED ON");
    risingEdgeDetected = false;
  }
  
  if (fallingEdgeDetected) {
    ledOn = false;
    onDuration = millis() - onStartTime;
    digitalWrite(ledPin, LOW);
    Serial.print("Falling edge detected -> LED OFF, on time = ");
    Serial.print(onDuration);
    Serial.println(" ms");
    fallingEdgeDetected = false;
  }

  delay(50);
}

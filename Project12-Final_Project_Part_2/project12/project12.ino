#include <Adafruit_MAX31856.h>

// --- Pin Definitions ---
const int DRDY_PIN = 5;        // Data Ready pin for MAX31856
const int CS_PIN = 4;          // Chip Select pin for MAX31856

// --- Object Initialization ---
Adafruit_MAX31856 maxthermo = Adafruit_MAX31856(CS_PIN); // Initialize Thermocouple (Hardware SPI)

// --- Timing Variables ---
unsigned long lastTempReadTime = 0;
const unsigned long tempReadInterval = 3000;    // Read temperature every 3000ms (3 seconds)

void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10); // Wait for Serial connection
  
  Serial.println("Initializing temperature sensor...");

  // Initialize MAX31856
  pinMode(DRDY_PIN, INPUT);
  if (!maxthermo.begin()) {
    Serial.println("Could not initialize thermocouple.");
    while (1) delay(10);
  }

  // Configure Thermocouple (Type K)
  maxthermo.setThermocoupleType(MAX31856_TCTYPE_K);
  maxthermo.setConversionMode(MAX31856_CONTINUOUS); // Enable continuous conversions

  Serial.println("System Ready! Logging temperature every 3 seconds...");
}

void loop() {
  // Check if 3 seconds have passed
  if (millis() - lastTempReadTime >= tempReadInterval) {
    // Check if a conversion is ready
    if (digitalRead(DRDY_PIN) == LOW) {
      float temperature = maxthermo.readThermocoupleTemperature();
      
      Serial.print("Current Temp: ");
      Serial.print(temperature);
      Serial.println(" C");
      
      lastTempReadTime = millis(); // Reset the timer
    }
  }
}
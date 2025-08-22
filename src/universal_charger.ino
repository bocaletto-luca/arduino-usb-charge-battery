#include <SPI.h>
#include <Wire.h>
#include <Adafruit_INA219.h>

// Pin definitions
const int CS_PIN        = 10;   // Chip Select for MCP41010
const int SETPOINT_PIN  = A0;   // 10 kΩ pot for voltage setpoint
const int BUTTON_PIN    = 2;    // Push-button input (INPUT_PULLUP)

// Charger configuration
const float MAX_OUTPUT_V = 12.0;   // Maximum desired output voltage (V)
const int   POT_STEPS    = 127;    // MCP41010 has 0–127 wiper positions
const float Kp           = 5.0;    // Proportional gain for coarse tuning

Adafruit_INA219 ina219;
int    wiperPosition = POT_STEPS / 2;
bool   charging      = false;
unsigned long lastAdjust = 0;

void setup() {
  Serial.begin(115200);
  while (!Serial);

  // SPI setup for digital potentiometer
  pinMode(CS_PIN, OUTPUT);
  digitalWrite(CS_PIN, HIGH);
  SPI.begin();
  SPI.setDataMode(SPI_MODE0);
  SPI.setClockDivider(SPI_CLOCK_DIV4);

  // INA219 high-side sensor
  ina219.begin();
  ina219.setCalibration_32V_2A();

  // Button
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // Initialize wiper to mid-scale
  setDigitalPot(wiperPosition);
  Serial.println("Universal Charger Initialized");
}

void loop() {
  // Toggle charging on button press
  static bool lastButton = HIGH;
  bool buttonState = digitalRead(BUTTON_PIN);
  if (lastButton == HIGH && buttonState == LOW) {
    charging = !charging;
    Serial.print("Charging ");
    Serial.println(charging ? "Started" : "Stopped");
    delay(50);  // Simple debounce
  }
  lastButton = buttonState;

  if (charging) {
    // Read desired voltage
    float raw = analogRead(SETPOINT_PIN) / 1023.0;
    float setV = raw * MAX_OUTPUT_V;

    // Read actual voltage
    float busV = ina219.getBusVoltage_V();  

    // Simple proportional control every 100 ms
    if (millis() - lastAdjust > 100) {
      float error = setV - busV;
      int   delta = round(error * Kp);
      wiperPosition = constrain(wiperPosition + delta, 0, POT_STEPS);
      setDigitalPot(wiperPosition);
      lastAdjust = millis();
    }

    // Telemetry
    float current_mA = ina219.getCurrent_mA();
    Serial.print("Set: "); Serial.print(setV, 2);   Serial.print(" V, ");
    Serial.print("Act: "); Serial.print(busV, 2);   Serial.print(" V, ");
    Serial.print("I: ");   Serial.print(current_mA); Serial.print(" mA, ");
    Serial.print("Wiper: "); Serial.println(wiperPosition);
  }

  delay(50);
}

/**
 * Write a new wiper value to the MCP41010 (volatile only).
 * Command byte: 0b0001_0000 (0x10) for POT0, followed by the data byte.
 */
void setDigitalPot(int value) {
  digitalWrite(CS_PIN, LOW);
  SPI.transfer(0x10);
  SPI.transfer(value & 0x7F);
  digitalWrite(CS_PIN, HIGH);
}

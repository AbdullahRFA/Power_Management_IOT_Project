#include <ZMPT101B.h>

#define ZMPT101B_PIN 34      // ADC pin connected to ZMPT101B output
#define SENSITIVITY 500.0f   // Calibration factor for your ZMPT101B module
#define FREQUENCY 60.0       // Frequency of AC source (60 Hz or 50 Hz)

ZMPT101B voltageSensor(ZMPT101B_PIN, FREQUENCY);

void setup() {
  Serial.begin(115200);
  
  // Initialize the ZMPT101B sensor
  voltageSensor.setSensitivity(SENSITIVITY); 
  Serial.println("ZMPT101B sensor initialized.");
}

void loop() {
  // Read RMS voltage from the sensor
  float rmsVoltage = voltageSensor.getRmsVoltage();

  // Print voltage reading with context
  Serial.print("Measured AC Voltage (RMS): ");
  Serial.print(rmsVoltage);
  Serial.println(" V");

  // Delay for stability
  delay(500);
}
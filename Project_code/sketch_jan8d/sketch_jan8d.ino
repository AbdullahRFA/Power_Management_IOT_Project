#include "ACS712.h"
#include <ZMPT101B.h>

// ACS712 setup
#define ACS712_PIN 34          // GPIO pin connected to ACS712
ACS712 ACS(ACS712_PIN, 3.3, 4095, 125);  // Parameters: analog pin, Vref, ADC resolution, sensor sensitivity
int calibration_factor = 120; // Calibration factor for ACS712

// ZMPT101B setup
#define ZMPT101B_PIN 35        // GPIO pin connected to ZMPT101B
#define SENSITIVITY 500.0f     // Sensitivity for ZMPT101B calibration
#define FREQUENCY 60.0         // Frequency of AC source (60 Hz or 50 Hz)
ZMPT101B voltageSensor(ZMPT101B_PIN, FREQUENCY);

void setup() {
  Serial.begin(115200);

  // Initialize ACS712
  Serial.println("Initializing ACS712 Current Sensor...");
  
  // Initialize ZMPT101B
  Serial.println("Initializing ZMPT101B Voltage Sensor...");
  voltageSensor.setSensitivity(SENSITIVITY); 
}

void loop() {
  // Read current using ACS712
  float current = readCurrent();

  // Read voltage using ZMPT101B
  float rmsVoltage = voltageSensor.getRmsVoltage();

  // Display the results
  Serial.print("Measured AC Current: ");
  Serial.print(current);
  Serial.println(" mA");

  Serial.print("Measured AC Voltage (RMS): ");
  Serial.print(rmsVoltage);
  Serial.println(" V");

  // Add a delay for stability
  delay(1000);
}

float readCurrent() {
  float average = 0;

  // Take multiple readings to average out noise
  for (int i = 0; i < 100; i++) {
    average += ACS.mA_AC();
  }

  // Calculate the average current and adjust for calibration
  float mA = (abs(average / 100.0) - calibration_factor);

  // Handle noise below a threshold
  if (mA <= 5) mA = 0;

  return mA;  // Return the measured current
}
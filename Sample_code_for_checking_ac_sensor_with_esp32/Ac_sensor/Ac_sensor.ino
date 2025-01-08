#include "ACS712.h"

ACS712 ACS(34, 3.3, 4095, 125);  // Parameters: analog pin, Vref, ADC resolution, sensor sensitivity
int calibration_factor = 120;

void setup() {
  Serial.begin(115200);
  Serial.println("ACS712 Current Measurement Initialized");
}

void loop() {
  float current = readCurrent();

  // Print the measured current
  Serial.print("Current: ");
  Serial.print(current);
  Serial.println(" mA");

  delay(1000);  // Delay for readability
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
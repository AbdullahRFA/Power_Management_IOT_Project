#include "ACS712.h"
#include <ZMPT101B.h>
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

// ACS712 setup
#define ACS712_PIN 34          // GPIO pin connected to ACS712
ACS712 ACS(ACS712_PIN, 3.3, 4095, 125);  // Parameters: analog pin, Vref, ADC resolution, sensor sensitivity
int calibration_factor = 120; // Calibration factor for ACS712

// ZMPT101B setup
#define ZMPT101B_PIN 35        // GPIO pin connected to ZMPT101B
#define SENSITIVITY 500.0f     // Sensitivity for ZMPT101B calibration
#define FREQUENCY 60.0         // Frequency of AC source (60 Hz or 50 Hz)
ZMPT101B voltageSensor(ZMPT101B_PIN, FREQUENCY);

// Blynk credentials
#define BLYNK_TEMPLATE_ID "TMPL6gGknfx6g"
#define BLYNK_TEMPLATE_NAME "Power management"
#define BLYNK_AUTH_TOKEN "B0le1amzQ3WStH549UnJuK1kOb_ihr1V"

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "322";
char pass[] = "murtazafy";

void setup() {
  Serial.begin(115200);

  // Initialize WiFi and Blynk
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected!");

  Serial.println("Connecting to Blynk...");
  Blynk.begin(auth, ssid, pass);

  // Initialize ACS712
  Serial.println("Initializing ACS712 Current Sensor...");
  
  // Initialize ZMPT101B
  Serial.println("Initializing ZMPT101B Voltage Sensor...");
  voltageSensor.setSensitivity(SENSITIVITY); 
}

void loop() {
  Blynk.run();

  // Read current using ACS712
  float current = readCurrent()*1000;

  // Read voltage using ZMPT101B
  float rmsVoltage = voltageSensor.getRmsVoltage();

  // Send data to Blynk
  Blynk.virtualWrite(V0, rmsVoltage);  // Send voltage to Virtual Pin V0
  Blynk.virtualWrite(V1, current);    // Send current to Virtual Pin V1

  // Display the results in the serial monitor
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
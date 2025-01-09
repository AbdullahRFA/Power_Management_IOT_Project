/*************************************************************
  This is a simple demo of sending and receiving some data.
  Be sure to check out other examples!
 *************************************************************/

/* Fill-in information from Blynk Device Info here */
#define BLYNK_TEMPLATE_ID           "TMPL68ptrSiHP"
#define BLYNK_TEMPLATE_NAME         "Quickstart Template"
#define BLYNK_AUTH_TOKEN            "msQRzTZLjJ88__UYisl_ZtOga2YivDgD"

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
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

// WiFi credentials
char ssid[] = "322";
char pass[] = "murtazafy";

BlynkTimer timer;

// This function is called every time the Virtual Pin 0 state changes
BLYNK_WRITE(V0)
{
  // Set incoming value from pin V0 to a variable
  int value = param.asInt();

  // Update state
  Blynk.virtualWrite(V1, value);
}

// This function is called every time the device is connected to the Blynk Cloud
BLYNK_CONNECTED()
{
  // Change Web Link Button message to "Congratulations!"
  Blynk.setProperty(V3, "offImageUrl", "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations.png");
  Blynk.setProperty(V3, "onImageUrl",  "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations_pressed.png");
  Blynk.setProperty(V3, "url", "https://docs.blynk.io/en/getting-started/what-do-i-need-to-blynk/how-quickstart-device-was-made");
}

// This function sends Arduino's uptime every second to Virtual Pin 2
void myTimerEvent()
{
  // You can send any value at any time
  Blynk.virtualWrite(V2, millis() / 1000);
}

// Function to read current from ACS712 sensor
float readCurrent()
{
  float average = 0.0;

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
float readVoltage(){
  float average = 0.0;

  for(int i=0;i<100;i++){
    average += voltageSensor.getRmsVoltage();
  }

  average = (average / 100.0);
  return average;
}
void setup()
{
  // Debug console
  Serial.begin(115200);

  // Connect to WiFi and Blynk
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected!");

  Serial.println("Connecting to Blynk...");
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  // Initialize ACS712
  Serial.println("Initializing ACS712 Current Sensor...");
  
  // Initialize ZMPT101B
  Serial.println("Initializing ZMPT101B Voltage Sensor...");
  voltageSensor.setSensitivity(SENSITIVITY); 

  // Setup a function to be called every second
  timer.setInterval(1000L, myTimerEvent);
}

void loop()
{
  Blynk.run();
  timer.run();

  // Read current using ACS712
  float current = readCurrent();

  // Read voltage using ZMPT101B
   float rmsVoltage = readVoltage();
  // float rmsVoltage = voltageSensor.getRmsVoltage();

  // Send data to Blynk
  Blynk.virtualWrite(V0, rmsVoltage);  // Send voltage to Virtual Pin V0
  Blynk.virtualWrite(V1, current);     // Send current to Virtual Pin V1

  // Display the results in the serial monitor
  Serial.print("Measured AC Current: ");
  Serial.print(current);
  Serial.println(" mA");

  Serial.print("Measured AC Voltage (RMS): ");
  Serial.print(rmsVoltage);
  Serial.println(" V");

  // Add a delay for stability
  // delay(1000);
}
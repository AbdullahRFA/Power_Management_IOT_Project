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
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED display setup (SDA = 21, SCL = 22 for ESP32)
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// ACS712 setup
#define ACS712_PIN 34          // GPIO pin connected to ACS712
ACS712 ACS(ACS712_PIN, 3.3, 4095, 125);  // Parameters: analog pin, Vref, ADC resolution, sensor sensitivity
int calibration_factor = 120; // Calibration factor for ACS712

// ZMPT101B setup
#define ZMPT101B_PIN 35        // GPIO pin connected to ZMPT101B
#define SENSITIVITY 500.0f     // Sensitivity for ZMPT101B calibration
#define FREQUENCY 60.0         // Frequency of AC source (60 Hz or 50 Hz)
ZMPT101B voltageSensor(ZMPT101B_PIN, FREQUENCY);

// Relay setup
#define RELAY_PIN 2            // GPIO pin connected to the relay module
bool relayState = false;       // To store the current state of the relay

// WiFi credentials
char ssid[] = "322";
char pass[] = "murtazafy";

BlynkTimer timer;

// Variables for power and energy calculation
float power = 0.0;      // Power in watts
float energy = 0.0;     // Energy in kWh (units)
unsigned long lastUpdateTime = 0;

// Function to calculate power and energy
  void calculatePowerAndEnergy(float current, float voltage) {
  // Calculate power (in watts)
  power = voltage * current / 1000.0; // Divide by 1000 to convert mA to A

  float K_power = (power / 1000.0);
  float units = K_power * 1;
  float per_unit_cost = (units * 12.67);

  // Send power and energy to Blynk
  Blynk.virtualWrite(V5, power);  // Send power to Virtual Pin V5
  Blynk.virtualWrite(V6, units); // Send energy (kWh) to Virtual Pin V6
  Blynk.virtualWrite(V3, per_unit_cost); // Send energy (kWh) to Virtual Pin V6

  // Display the results in the serial monitor
  Serial.print("Power: ");
  Serial.print(power);
  Serial.println(" W");

  Serial.print("Energy: ");
  Serial.print(units);
  Serial.println(" kWh");

  Serial.print("Cost_Per_Unit: ");
  Serial.print(per_unit_cost);
  Serial.println(" TK");
  
  display.print("Power: ");
  display.print(power);
  display.println(" W");

  display.print("Energy: ");
  display.print(units);
  display.println(" kWh");

  display.print("Cost: ");
  display.print(per_unit_cost);  // Display cost in TK
  display.println(" TK");

  display.display();  // Update the display with the new data


}

// Function to control relay (connected to Virtual Pin V4)
BLYNK_WRITE(V4) {
  int buttonState = param.asInt(); // Read the button state from Blynk
  if (buttonState == 1) {
    digitalWrite(RELAY_PIN, LOW);  // Turn relay ON (LOW activates the relay)
    relayState = true;            // Update relay state
  } else {
    digitalWrite(RELAY_PIN, HIGH); // Turn relay OFF (HIGH deactivates the relay)
    relayState = false;            // Update relay state
  }
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
  if (mA <= 70) mA = 0;

  return mA;  // Return the measured current
}

// Function to read voltage using ZMPT101B
float readVoltage() {
  float average_voltage = 0.0;

  for (int i = 0; i < 100; i++) {
    average_voltage += voltageSensor.getRmsVoltage();
  }

  average_voltage = (average_voltage / 100.0);
  if (average_voltage <= 10) average_voltage = 0;
  return average_voltage;
}

void setup()
{
  // Debug console
  Serial.begin(115200);

  // Configure relay pin as output
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW); // Ensure relay is OFF initially

 // Initialize the OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {  // 0x3C is the I2C address of the OLED
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);  // Don't proceed, loop forever
  }

  display.display();  
  delay(2000);  // Pause for 2 seconds

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

  // Display the result on serial monitor
  Serial.print("Voltage : ");
  Serial.print(rmsVoltage);
  Serial.println(" V");

  Serial.print("Current : ");
  Serial.print(current);
  Serial.println(" mA");


  // Send data to Blynk
  Blynk.virtualWrite(V0, rmsVoltage);  // Send voltage to Virtual Pin V0
  Blynk.virtualWrite(V1, current);     // Send current to Virtual Pin V1

  // Display the results on the OLED
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("Voltage: ");
  display.print(rmsVoltage);
  display.println(" V");

 display.setCursor(0, 2);
  display.print("Current: ");
  display.print(current);
  display.println(" mA");

  // this function calculatePowerAndEnergy
  calculatePowerAndEnergy(current, rmsVoltage);

  delay(1000);
}
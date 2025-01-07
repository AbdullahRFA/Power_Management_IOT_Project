#include "WiFi.h"

void setup() {
  Serial.begin(115200); // Start Serial communication
  delay(1000);

  Serial.println("Starting Wi-Fi scan...");
  WiFi.mode(WIFI_STA); // Set the ESP32 as a Wi-Fi station
  WiFi.disconnect();
  delay(100);
}

void loop() {
  Serial.println("Scanning for networks...");
  int numNetworks = WiFi.scanNetworks();

  if (numNetworks == 0) {
    Serial.println("No networks found.");
  } else {
    Serial.println("Networks found:");
    for (int i = 0; i < numNetworks; ++i) {
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.println(" dBm)");
    }
  }
  delay(10000); // Wait 10 seconds before scanning again
}
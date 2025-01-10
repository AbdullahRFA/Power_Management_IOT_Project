# Power Management System with ESP32, ACS712, and ZMPT101B

# Overview

This project demonstrates a Power Management System using the ESP32 microcontroller, current sensor (ACS712), voltage sensor (ZMPT101B), OLED display (SSD1306), and Blynk IoT platform. It monitors voltage, current, power, and energy usage, and allows relay control via Blynk. Additionally, it calculates energy cost based on a specified per-unit cost.

# Features
	1.	Real-time Monitoring:
	•	Measures voltage (V), current (mA), and power (W).
	•	Calculates energy (kWh) and cost (TK).
	2.	OLED Display:
	•	Displays measured parameters (Voltage, Current, Power, Energy, and Cost) in real-time.
	3.	IoT Integration:
	•	Sends data to the Blynk IoT app for remote monitoring and control.
	4.	Relay Control:
	•	Controls a relay module using the Blynk app.
	5.	Data Logging:
	•	Logs measurements for easy debugging via the Serial Monitor.

# Components Used

# Hardware:
	1.	ESP32: Main microcontroller for processing data and WiFi connectivity.
	2.	ACS712 (Current Sensor): Measures current in the system.
	3.	ZMPT101B (Voltage Sensor): Measures AC voltage.
	4.	SSD1306 OLED Display: Displays real-time measurements.
	5.	Relay Module: Toggles connected electrical devices ON/OFF.
	6.	Resistors and Capacitors: For noise filtering and stability.

# Software:
	1.	Arduino IDE: For programming the ESP32.
	2.	Blynk IoT Platform: For remote data monitoring and relay control.
	3.	Libraries:
	•	WiFi.h and BlynkSimpleEsp32.h for Blynk and WiFi integration.
	•	ACS712.h and ZMPT101B.h for sensor readings.
	•	Adafruit_GFX.h and Adafruit_SSD1306.h for OLED display.

# How It Works

# Core Functionalities:
	1.	Voltage Measurement:
	•	The ZMPT101B sensor reads the RMS voltage from the AC source.
	•	Sensitivity is calibrated using the setSensitivity() method.
	2.	Current Measurement:
	•	The ACS712 sensor reads the AC current flowing through the load.
	•	Noise is filtered, and a calibration factor is applied for accuracy.
	3.	Power & Energy Calculation:
	•	Power (￼) = Voltage (￼) × Current (￼).
	•	Energy (￼) = Power × Time.
	•	Cost is calculated using a predefined per-unit cost (12.67 TK).
	4.	Relay Control:
	•	A relay is toggled using Virtual Pin ￼ on the Blynk app.
	•	ON: Activates relay to power the load.
	•	OFF: Deactivates relay.
	5.	OLED Display:
	•	Displays Voltage, Current, Power, Energy, and Cost in real-time.
	6.	Blynk IoT Integration:
	•	Sends measurements to Blynk app on Virtual Pins:
	•	￼: Voltage
	•	￼: Current
	•	￼: Power
	•	￼: Energy
	•	￼: Cost
	•	Controls relay via ￼.

# Pin Configuration

# 	Component				Pin					Description
	ACS712 (Current Sensor)			GPIO 34					Reads current values.
	ZMPT101B (Voltage Sensor)		GPIO 35					Reads voltage values.
	OLED Display (SSD1306)			SDA: GPIO 21				I2C Data line.
						SCL: GPIO 22				I2C Clock line.
	Relay Module				GPIO 2					Toggles the relay.

# Setup Instructions
	1.	Hardware Setup:
	•	Connect the ACS712, ZMPT101B, OLED Display, and Relay to the ESP32 as per the pin configuration.
	2.	Install Libraries:
	•	Install required libraries via the Arduino Library Manager:
	•	Blynk
	•	Adafruit GFX
	•	Adafruit SSD1306
	•	ACS712
	•	ZMPT101B
	3.	Code Configuration:
	•	Replace the WiFi credentials:

# char ssid[] = "YourWiFiSSID";
# char pass[] = "YourWiFiPassword";


	•	Replace the Blynk Auth Token:

# define BLYNK_AUTH_TOKEN "YourBlynkAuthToken"


	4.	Upload Code:
	•	Compile and upload the code to your ESP32 using the Arduino IDE.
	5.	Blynk App Setup:
	•	Add widgets for displaying Voltage, Current, Power, Energy, and Cost.
	•	Add a Button widget for Relay control on Virtual Pin ￼.
	6.	Calibration:
	•	Adjust the calibration factors for ACS712 and ZMPT101B as per your hardware setup for accurate readings.

# Blynk Virtual Pins

# Virtual Pin	Parameter	Description
	V0	Voltage (V)	Real-time voltage readings.
	V1	Current (mA)	Real-time current readings.
	V5	Power (W)	Power consumption in watts.
	V6	Energy (kWh)	Total energy consumption.
	V3	Cost (TK)	Energy cost in TK.
	V4	Relay Control	Button to toggle relay state.
	V2	Uptime		Displays system uptime in seconds.

# Example Output

# Serial Monitor:

	Voltage: 220.0 V
	Current: 500.0 mA
	Power: 110.0 W
	Energy: 0.11 kWh
	Cost_Per_Unit: 1.39 TK

# OLED Display:

	Power Management
	Voltage: 220.0 V
	Current: 500.0 mA
	Power: 110.0 W
	Energy: 0.11 kWh
	Cost: 1.39 TK

# Future Enhancements
	•	Integrate more sensors for advanced monitoring (e.g., temperature, humidity).
	•	Implement data logging using an SD card or cloud storage.
	•	Add energy-saving modes or automation based on thresholds.

# Author

# Abdullah Nazmus-Sakib
A passionate IoT developer and tech enthusiast.

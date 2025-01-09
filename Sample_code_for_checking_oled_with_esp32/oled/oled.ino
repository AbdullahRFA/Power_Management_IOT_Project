#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels
#define OLED_RESET    -1  // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  // Start serial communication for debugging purposes
  Serial.begin(115200);

  // Initialize the OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {  // 0x3C is the I2C address of the OLED
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);  // Don't proceed, loop forever
  }

  // Clear the buffer
  display.clearDisplay();

  // Print the initialization message
  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner
  display.println(F("OLED Display Test"));
  display.display();           // Show the message on the display
  delay(2000);                 // Wait for 2 seconds
}

void loop() {
  static int counter = 0;

  // Clear the display
  display.clearDisplay();

  // Display a counter value
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("Counter: ");
  display.println(counter);

  display.display();  // Show updated content on OLED

  // Debug output to the serial monitor
  Serial.print("Counter: ");
  Serial.println(counter);

  counter++;
  delay(1000);  // Update every second
}
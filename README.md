# Smart Pill Dispenser with IoT Health Monitoring 💊

This project is an ESP32-based smart pill dispenser with a web-based scheduler, OLED display, and IoT integration with ThingSpeak for monitoring  health.

The system allows users to manage medication schedules via a web app served directly from the ESP32. It physically dispenses pills at the correct times using a motor-driven mechanism with piezo sensor feedback for reliability.

---

##  Key Features

* **Local Web-Based Scheduler**: A responsive web app to create, edit, and sync complex medication schedules directly to the ESP32.
* **Private & Secure**: All schedule data is stored locally in the browser and on the device, requiring no cloud accounts for core functionality.
* **OLED Status Display**: An onboard screen shows the current time, next pill, and real-time dispensing status.
* **Reliable Dispensing**: A piezo sensor confirms each pill drop, with an automatic retry mechanism for failed attempts.
* **IoT Health Monitoring**: After each dispense, the ESP32 sends the number of motor attempts to a **ThingSpeak** channel, allowing you to visualize the device's mechanical health over time.
* **User Alerts**: Includes a buzzer for audible alarms and a physical button to acknowledge that a dose has been taken.

---

##  Hardware & Software

### Hardware

* ESP32 Dev Kit
* SSD1306 128x64 OLED Display (I2C)
* 2x DC Geared Motors & Motor Driver
* Piezoelectric Sensor (for drop detection)
* Buzzer & Push Button

### Software

This project is built using **PlatformIO** with the Arduino framework. Key libraries include `ESPAsyncWebServer`, `ArduinoJson`, `Adafruit_SSD1306`, and `ThingSpeak`.

---

##  Getting Started

### 1. Configure Credentials

Before uploading, you must edit `src/main.cpp` with your WiFi and ThingSpeak details:

```cpp
// --- Wi-Fi Credentials ---
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

// --- ThingSpeak Configuration ---
unsigned long myChannelNumber = 1234567; // Your Channel ID
const char * myWriteAPIKey = "YOUR_WRITE_API_KEY"; // Your Write API Key

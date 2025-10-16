// main.cpp
// ESP32 Firmware for the Pill Dispenser Scheduler
// Integrates ThingSpeak for system health monitoring.

// 1. INCLUDE LIBRARIES
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include "time.h"
#include "webpage.h" 
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "ThingSpeak.h"      // <<-- ADDED
#include <WiFiClient.h>      // <<-- ADDED


// 2. CONFIGURATION
// --- Wi-Fi Credentials ---
const char* ssid = "ARNAVA_2.4GHz";
const char* password = "HiddenPassword2";

// --- ThingSpeak Configuration ---
unsigned long myChannelNumber = 0; // <<-- PASTE YOUR CHANNEL ID HERE
const char * myWriteAPIKey = "YOUR_WRITE_API_KEY"; // <<-- PASTE YOUR WRITE API KEY HERE
WiFiClient client;

// --- OLED Display & Hardware Pins ---
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
#define SCREEN_ADDRESS 0x3C
#define I2C_SDA 23
#define I2C_SCL 22
#define BUZZER_PIN 27
#define LED_WIFI_PIN 2
#define ACK_BUTTON_PIN 14
#define MOTOR1_DIR_PIN 25
#define MOTOR1_SPD_PIN 12
#define MOTOR2_DIR_PIN 26
#define MOTOR2_SPD_PIN 13
#define PIEZO_SENSOR_PIN 34
const int PIEZO_THRESHOLD = 500;
const int ms1=40;
const int ms2=60;
const int PWM_FREQ = 30000;
const int PWM_RESOLUTION = 8;
int MOTOR_SPEED1;
int MOTOR_SPEED2;
const int PWM_CHANNEL_M1 = 0;
const int PWM_CHANNEL_M2 = 1;
const int DISPENSE_TIME_MS1 = 625;
const int DISPENSE_TIME_MS2 = 775;
const int RETURN_TIME_MS = 300;
const int RETRY_DELAY_MS = 5000;
const int MAX_DISPENSE_ATTEMPTS = 5;
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 19800;
const int   daylightOffset_sec = 0;

// 3. GLOBAL VARIABLES
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
struct Schedule { char id[20], pillName[50]; int compartmentNum; char startDate[11], endDate[11], mode[12]; int frequencyDays; char days[7][4]; int daysCount; char times[5][6]; int timesCount; };
Schedule schedules[10];
int scheduleCount = 0;
struct DispenserStatus { bool isDispensing = false, dispensedConfirmed = false; char pill[50] = "", pillTime[6] = ""; int compartment = 0; } currentStatus;
char nextPillName[50] = "No Schedule", nextPillTime[6] = "";
char takenDosesToday[20][6];
int takenDoseCount = 0, lastCheckedDay = -1;
AsyncWebServer server(80);
unsigned long lastScheduleCheckTime = 0, lastOledUpdateTime = 0, lastButtonPressTime = 0;
const unsigned long debounceDelay = 200;

// 4. HELPER FUNCTIONS

void displayMessage(const char* line1, const char* line2 = nullptr, int duration = 0) {
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    int16_t x1, y1; uint16_t w1, h1;
    display.getTextBounds(line1, 0, 0, &x1, &y1, &w1, &h1);
    display.setCursor((SCREEN_WIDTH - w1) / 2, 15);
    display.println(line1);
    if (line2) {
        display.getTextBounds(line2, 0, 0, &x1, &y1, &w1, &h1);
        display.setCursor((SCREEN_WIDTH - w1) / 2, 36);
        display.println(line2);
    }
    display.display();
    if (duration > 0) {
        delay(duration);
        display.clearDisplay();
        display.display();
    }
}

void findNextPill() {
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
        strcpy(nextPillName, "Time Sync Error");
        strcpy(nextPillTime, "");
        return;
    }
    long closestDiff = -1;
    char tempNextPillName[50] = "NoSchedule";
    char tempNextPillTime[6] = "";
    long nowInMinutes = timeinfo.tm_hour * 60 + timeinfo.tm_min;
    for (int i = 0; i < scheduleCount; i++) {
        for (int t = 0; t < schedules[i].timesCount; t++) {
            int schedHour, schedMin;
            sscanf(schedules[i].times[t], "%d:%d", &schedHour, &schedMin);
            long schedInMinutes = schedHour * 60 + schedMin;
            long diff = schedInMinutes - nowInMinutes;
            if (diff >= 0) {
                if (closestDiff == -1 || diff < closestDiff) {
                    closestDiff = diff;
                    strcpy(tempNextPillName, schedules[i].pillName);
                    strcpy(tempNextPillTime, schedules[i].times[t]);
                }
            }
        }
    }
    strcpy(nextPillName, tempNextPillName);
    strcpy(nextPillTime, tempNextPillTime);
}

void updateIdleDisplay() {
    display.clearDisplay();
    struct tm timeinfo;
    if (getLocalTime(&timeinfo)) {
        char timeString[15];
        strftime(timeString, sizeof(timeString), "%a %H:%M", &timeinfo);
        display.setTextSize(1);
        display.setTextColor(SSD1306_WHITE);
        int16_t x1, y1; uint16_t w1, h1;
        display.getTextBounds(timeString, 0, 0, &x1, &y1, &w1, &h1);
        display.setCursor((SCREEN_WIDTH - w1) / 2, 2);
        display.println(timeString);
    }
    display.setTextSize(1);
    display.setCursor(0, 20);
    display.println("Next Pill:");
    display.setTextSize(2);
    int16_t x1, y1; uint16_t w1, h1;
    display.getTextBounds(nextPillName, 0, 0, &x1, &y1, &w1, &h1);
    display.setCursor((SCREEN_WIDTH - w1) / 2, 35);
    display.println(nextPillName);
    display.setTextSize(1);
    display.getTextBounds(nextPillTime, 0, 0, &x1, &y1, &w1, &h1);
    display.setCursor((SCREEN_WIDTH - w1) / 2, 52);
    display.println(nextPillTime);
    display.display();
}

void stopMotors() {
    ledcWrite(PWM_CHANNEL_M1, 0);
    ledcWrite(PWM_CHANNEL_M2, 0);
    digitalWrite(MOTOR1_DIR_PIN, LOW);
    digitalWrite(MOTOR2_DIR_PIN, LOW);
}

void turnOffAlarm() {
    digitalWrite(BUZZER_PIN, LOW);
    bool alreadyLogged = false;
    for (int i = 0; i < takenDoseCount; i++) {
        if (strcmp(takenDosesToday[i], currentStatus.pillTime) == 0) {
            alreadyLogged = true;
            break;
        }
    }
    if (!alreadyLogged && takenDoseCount < 20) {
        strcpy(takenDosesToday[takenDoseCount], currentStatus.pillTime);
        takenDoseCount++;
        Serial.printf("Logged dose for time %s as taken.\n", currentStatus.pillTime);
    }
    currentStatus.dispensedConfirmed = false;
    display.clearDisplay();
    display.display();
}

int readPiezo() {
    long total = 0;
    for (int i = 0; i < 5; i++) { total += analogRead(PIEZO_SENSOR_PIN); delay(1); }
    return total / 5;
}

// MODIFIED: Returns the number of attempts (1-5), or 0 on failure.
int runMotorCycle(int compartment) {
    if (compartment < 1 || compartment > 2) return 0;
    int dirPin = (compartment == 1) ? MOTOR1_DIR_PIN : MOTOR2_DIR_PIN;
    int pwmChannel = (compartment == 1) ? PWM_CHANNEL_M1 : PWM_CHANNEL_M2;
    int MOTOR_SPEED = (compartment == 1) ? MOTOR_SPEED1 : MOTOR_SPEED2;
    int DISPENSE_TIME_MS = (compartment == 1) ? DISPENSE_TIME_MS1 : DISPENSE_TIME_MS2;
    bool dispenseSuccess = false;
    int attempt;
    for (attempt = 1; attempt <= MAX_DISPENSE_ATTEMPTS; ++attempt) {
        Serial.printf("Dispense attempt #%d for compartment %d\n", attempt, compartment);
        digitalWrite(dirPin, LOW);
        ledcWrite(pwmChannel, MOTOR_SPEED);
        unsigned long motorStartTime = millis();
        while (millis() - motorStartTime < 1000) {
            if (readPiezo() > PIEZO_THRESHOLD) {
                dispenseSuccess = true;
                break;
            }
            yield();
        }
        ledcWrite(pwmChannel, 0);
        if (dispenseSuccess) {
            Serial.println("Success: Pill detected by Piezo sensor.");
            break; 
        } else {
            Serial.println("Failure: Pill not detected. Reversing motor.");
            digitalWrite(dirPin, LOW);
            ledcWrite(pwmChannel, MOTOR_SPEED);
            delay(RETURN_TIME_MS);
            stopMotors();
            if (attempt < MAX_DISPENSE_ATTEMPTS) {
                Serial.printf("Waiting for %dms before retrying.\n", RETRY_DELAY_MS);
                delay(RETRY_DELAY_MS);
            }
        }
    }
    stopMotors();
    return dispenseSuccess ? attempt : 0;
}

void testpill(int compartment) {
    currentStatus.isDispensing=true;
    int dirPin = (compartment == 1) ? MOTOR1_DIR_PIN : MOTOR2_DIR_PIN;
    int pwmChannel = (compartment == 1) ? PWM_CHANNEL_M1 : PWM_CHANNEL_M2;
    int MOTOR_SPEED = (compartment == 1) ? MOTOR_SPEED1 : MOTOR_SPEED2;
    int DISPENSE_TIME_MS = (compartment == 1) ? DISPENSE_TIME_MS1 : DISPENSE_TIME_MS2;
    ledcWrite(pwmChannel, MOTOR_SPEED);
    delay(DISPENSE_TIME_MS);
    ledcWrite(pwmChannel, 0);
    currentStatus.isDispensing=false;
}

// MODIFIED: Sends dispense attempts to ThingSpeak
void dispensePill(int compartment, const char* pillName, const char* pillTime) {
    Serial.printf("Scheduled dispense of %s from compartment %d at %s\n", pillName, compartment, pillTime);
    currentStatus.isDispensing = true;
    strcpy(currentStatus.pill, pillName);
    strcpy(currentStatus.pillTime, pillTime);
    currentStatus.compartment = compartment;
    
    int attempts = runMotorCycle(compartment);
    
    currentStatus.isDispensing = false;
    if (attempts > 0) {
        displayMessage("Pill", "Dispensed");
        currentStatus.dispensedConfirmed = true;
        digitalWrite(BUZZER_PIN, HIGH);

        // --- NEW: Send System Health Data to ThingSpeak ---
        Serial.println("Sending data to ThingSpeak...");
        ThingSpeak.setField(1, attempts);
        int httpCode = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
        if(httpCode == 200) {
          Serial.println("ThingSpeak update successful.");
        } else {
          Serial.println("ThingSpeak update failed. Error: " + String(httpCode));
        }
        // --- End of ThingSpeak Code ---

    } else {
        displayMessage("Pill Not", "Dispensed", 10000);
    }
}

void checkSchedules() {
    if (currentStatus.dispensedConfirmed || currentStatus.isDispensing) return;
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) return;
    char todayStr[4]; strftime(todayStr, 4, "%a", &timeinfo);
    char nowTimeStr[6]; strftime(nowTimeStr, 6, "%H:%M", &timeinfo);
    for (int i = 0; i < scheduleCount; i++) {
        bool isToday = false;
        if (strcmp(schedules[i].mode, "days") == 0) {
            for (int j = 0; j < schedules[i].daysCount; j++) {
                if (strcmp(schedules[i].days[j], todayStr) == 0) {
                    isToday = true;
                    break;
                }
            }
        } else { isToday = true; }
        if (isToday) {
            for (int t = 0; t < schedules[i].timesCount; t++) {
                bool alreadyTaken = false;
                for (int d = 0; d < takenDoseCount; d++) {
                    if (strcmp(takenDosesToday[d], schedules[i].times[t]) == 0) {
                        alreadyTaken = true;
                        break;
                    }
                }
                if (strcmp(schedules[i].times[t], nowTimeStr) == 0 && !alreadyTaken) {
                    dispensePill(schedules[i].compartmentNum, schedules[i].pillName, schedules[i].times[t]);
                    return;
                }
            }
        }
    }
}

// 5. WEB SERVER ENDPOINTS
void handleTestMotor1(AsyncWebServerRequest *request) { if (currentStatus.isDispensing || currentStatus.dispensedConfirmed) return request->send(409, "text/plain", "Busy."); Serial.println("--- MANUAL TEST: MOTOR 1 START ---"); testpill(1); request->send(200, "text/plain", "Motor 1 activated."); }
void handleTestMotor2(AsyncWebServerRequest *request) { if (currentStatus.isDispensing || currentStatus.dispensedConfirmed) return request->send(409, "text/plain", "Busy."); Serial.println("--- MANUAL TEST: MOTOR 2 START ---"); testpill(2); request->send(200, "text/plain", "Motor 2 activated."); }
void handleSchedulesRequest(AsyncWebServerRequest *request) { if (request->method() == HTTP_POST) request->send(200, "text/plain", "OK."); else request->send(405, "text/plain", "Method not allowed"); }

void handleScheduleBody(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
    if (index == 0) Serial.println("Receiving new schedules...");
    if (len == total) {
        StaticJsonDocument<8192> doc;
        if (deserializeJson(doc, (const char*)data, len)) { request->send(400, "text/plain", "Invalid JSON."); return; }
        JsonArray array = doc.as<JsonArray>();
        scheduleCount = 0;
        for (JsonObject obj : array) {
            if(scheduleCount >= 10) break;
            Schedule& s = schedules[scheduleCount];
            strncpy(s.id, obj["id"], sizeof(s.id) - 1);
            strncpy(s.pillName, obj["pillName"], sizeof(s.pillName) - 1);
            s.compartmentNum = obj["compartmentNum"];
            strncpy(s.startDate, obj["startDate"], sizeof(s.startDate) - 1);
            strncpy(s.endDate, obj["endDate"], sizeof(s.endDate) - 1);
            strncpy(s.mode, obj["mode"], sizeof(s.mode) - 1);
            s.frequencyDays = obj["frequencyDays"];
            JsonArray days = obj["days"];
            s.daysCount = 0;
            for(JsonVariant v : days) { if(s.daysCount >= 7) break; strncpy(s.days[s.daysCount++], v.as<const char*>(), 3); }
            JsonArray times = obj["times"];
            s.timesCount = 0;
            for(JsonVariant v : times) { if(s.timesCount >= 5) break; strncpy(s.times[s.timesCount++], v.as<const char*>(), 5); }
            Serial.printf("Loaded schedule for: %s\n", s.pillName);
            scheduleCount++;
        }
        request->send(200, "text/plain", "Schedules received and processed!");
    }
}

void handleStatus(AsyncWebServerRequest *request) {
    JsonDocument doc;
    doc["isDispensing"] = currentStatus.isDispensing;
    doc["dispensedConfirmed"] = currentStatus.dispensedConfirmed;
    doc["pill"] = currentStatus.pill;
    doc["compartment"] = currentStatus.compartment;
    doc["pillTime"] = currentStatus.pillTime;
    JsonArray takenTimes = doc.createNestedArray("takenToday");
    for (int i = 0; i < takenDoseCount; i++) takenTimes.add(takenDosesToday[i]);
    String output;
    serializeJson(doc, output);
    request->send(200, "application/json", output);
}

void handleDispenseAck(AsyncWebServerRequest *request) {
    Serial.println("Dose taken acknowledgement received from web.");
    turnOffAlarm();
    request->send(200, "text/plain", "ACK");
}

// 6. SETUP AND LOOP
void setup() {
    Serial.begin(115200);
    MOTOR_SPEED1 = map(ms1, 0, 100, 0, 255);
    MOTOR_SPEED2 = map(ms2, 0, 100, 0, 255);
    Wire.begin(I2C_SDA, I2C_SCL);
    if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) { Serial.println(F("SSD1306 allocation failed.")); }
    display.clearDisplay(); display.display();
    
    ThingSpeak.begin(client); // <<-- ADDED

    pinMode(BUZZER_PIN, OUTPUT); pinMode(LED_WIFI_PIN, OUTPUT); pinMode(ACK_BUTTON_PIN, INPUT_PULLUP);
    digitalWrite(BUZZER_PIN, LOW); digitalWrite(LED_WIFI_PIN, LOW);
    pinMode(MOTOR1_DIR_PIN, OUTPUT); pinMode(MOTOR2_DIR_PIN, OUTPUT);
    digitalWrite(MOTOR1_DIR_PIN, LOW); digitalWrite(MOTOR2_DIR_PIN, LOW);
    pinMode(PIEZO_SENSOR_PIN, INPUT);
    ledcSetup(PWM_CHANNEL_M1, PWM_FREQ, PWM_RESOLUTION); ledcSetup(PWM_CHANNEL_M2, PWM_FREQ, PWM_RESOLUTION);
    ledcAttachPin(MOTOR1_SPD_PIN, PWM_CHANNEL_M1); ledcAttachPin(MOTOR2_SPD_PIN, PWM_CHANNEL_M2);
    stopMotors();
    displayMessage("Connecting", "WiFi...");
    WiFi.begin(ssid, password);
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 50) { delay(500); attempts++; }
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\nWiFi connected."); Serial.print("IP address: "); Serial.println(WiFi.localIP());
        digitalWrite(LED_WIFI_PIN, HIGH);
        displayMessage("Syncing", "Time...");
        configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
        int sync_attempts = 0;
        while (!time(nullptr) && sync_attempts < 50) { delay(500); sync_attempts++; }
        if (time(nullptr)) Serial.println("\nTime successfully synchronized!");
        else displayMessage("Error", "Time Sync", 10000);
    } else {
        displayMessage("Error", "Connecting", 10000);
    }
    server.onNotFound([](AsyncWebServerRequest *request) { if (request->method() == HTTP_OPTIONS) request->send(200); else request->send(404, "Not found"); });
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "Content-Type");
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){ request->send_P(200, "text/html", index_html); });
    server.on("/schedules", HTTP_POST, handleSchedulesRequest, NULL, handleScheduleBody);
    server.on("/status", HTTP_GET, handleStatus);
    server.on("/dispense", HTTP_POST, handleDispenseAck);
    server.on("/test/motor1", HTTP_POST, handleTestMotor1);
    server.on("/test/motor2", HTTP_POST, handleTestMotor2);
    server.begin();
    Serial.println("HTTP server started.");
}

void loop() {
    unsigned long currentTime = millis();
    struct tm timeinfo;
    if (getLocalTime(&timeinfo)) {
        if (lastCheckedDay != timeinfo.tm_yday && timeinfo.tm_year > 100) {
            takenDoseCount = 0;
            lastCheckedDay = timeinfo.tm_yday;
        }
    }
    if (currentStatus.dispensedConfirmed) {
        if (digitalRead(ACK_BUTTON_PIN) == LOW && (currentTime - lastButtonPressTime > debounceDelay)) {
            lastButtonPressTime = currentTime;
            turnOffAlarm();
        }
    }
    if (currentTime - lastScheduleCheckTime >= 30000) {
        lastScheduleCheckTime = currentTime;
        if (!currentStatus.dispensedConfirmed && !currentStatus.isDispensing) {
            checkSchedules();
        }
    }
    if (currentTime - lastOledUpdateTime >= 5000) {
        lastOledUpdateTime = currentTime;
        if (!currentStatus.dispensedConfirmed && !currentStatus.isDispensing) {
            findNextPill();
            updateIdleDisplay();
        }
    }
}


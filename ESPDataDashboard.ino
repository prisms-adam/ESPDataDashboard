/*
 * ESPDataDashboard.ino v0.1
 * -------------------------
 * Created by: Adam Kemp, 2026
 * Description: Main entry point for the ESP8266 Data Dashboard. 
 * This file handles WiFi, NTP, and serves the Web API.
 * It now supports "Output Control" to trigger hardware (LEDs, Motors, etc.)
 * automatically when an experiment starts or stops.
 *
 * STUDENT TASKS:
 * 1. Configure WiFi (ssid and password).
 * 2. Define sensor reading functions (readAnalog, etc.).
 * 3. Register sensors in the 'sensors' array.
 * 4. Register outputs (LEDs, Motors) in the 'outputs' array.
 * 5. Adjust the GMT Offset if your time is incorrect.
 */

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <time.h>

// Import HTML/CSS/JS from tabs
#include "index_html.h"
#include "style_css.h"
#include "script_js.h"

/*  ==========================================
*   STUDENT TASK 1: CONFIGURE WIFI & TIME
*   ==========================================
*   
*   1) Update the ssid and password constants below
*/

const char* ssid = "PRIS_Student"; //Change this to your desired SSID
const char* password = "wearethebest1"; //Change this to the password for the desired SSID

// Time and web server configuration
// NTP Server settings for time (Requires Internet connection on the WiFi)
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = -14400; //Update to your current GMT offset
const int   daylightOffset_sec = 0;

ESP8266WebServer server(80);
bool isRecording = false;

// Function to acquire and format date
String getDateString() {
  time_t now;
  struct tm timeinfo;
  if (!time(&now)) return "1970-01-01"; 
  localtime_r(&now, &timeinfo);
  char dateString[11];
  sprintf(dateString, "%04d-%02d-%02d", timeinfo.tm_year + 1900, timeinfo.tm_mon + 1, timeinfo.tm_mday);
  return String(dateString);
}

// Function to acquire and format time
String getTimeString() {
  time_t now; struct tm timeinfo;
  if (!time(&now)) return "00:00:00";
  localtime_r(&now, &timeinfo);
  char buf[9]; sprintf(buf, "%02d:%02d:%02d", timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
  return String(buf);
}

/* ========================================== 
*  STUDENT TASK 2: SENSOR READING FUNCTIONS
*  ==========================================
*  
*  1) Define functions here to read your specific sensors.
*  2) Each function must return a float.
*/

float readAnalogA0() {
  return (float)analogRead(A0);
}

// EXAMPLE: If you add a second sensor, define it like this:

float readInternalTemp() {
    // Example: Read from a dummy or real sensor
    return 25.0 + (rand() % 100) / 10.0; 
}

/* ==========================================
*  STUDENT TASK 3: REGISTER SENSORS
*  ==========================================
*  
*  1) Review, edit, and add sensors below in the sensors[] array
*  2) Make sure you have created sensor reading functions in Task 2! 
*/

// The following structure establishes the sensor config format
struct Sensor {
  String id;
  String label;
  String unit;
  float (*readFunc)();
};

// Update the following array with each sensor's information
Sensor sensors[] = {
  { "analog", "Analog Input", "Raw", readAnalogA0 },
  //{ "temp",   "Temperature",  "°C",  readInternalTemp } // Uncomment to add more
};

const int sensorCount = sizeof(sensors) / sizeof(Sensor);

/* ==========================================
*  STUDENT TASK 4: REGISTER OUTPUTS
*  ==========================================
*  
*  1) Update the outputs[] arracy to contain your desired outputs. Use the following format as a guide:
*     Format: { "unique_id", "Display Label", Pin_Number, On_Value, isPWM }
*     On_Value: For Digital, 1 = HIGH. For PWM, 0 to 1023.
*  
*/

struct Output {
  String id;
  String label;
  int pin;
  int onValue;
  bool isPwm;
};

Output outputs[] = {
  //{ "led", "LED (D0)", D0, 1, false }, // Digital Output on D0
  //{ "fan", "Cooling Fan", D6, 512, true }   // PWM Output (50% speed) on D6
};
const int outputCount = sizeof(outputs) / sizeof(Output);

/* ==========================================
*  Hardware Control Logic
*  ==========================================
*/

void applyOutputState(bool active) {
  for (int i = 0; i < outputCount; i++) {
    int val = active ? outputs[i].onValue : 0;
    if (outputs[i].isPwm) {
      analogWrite(outputs[i].pin, val);
    } else {
      digitalWrite(outputs[i].pin, val ? HIGH : LOW);
    }
  }
}

/* ==========================================
*  Web Server Handlers
*  ========================================== 
*/

void handleRoot() { server.send(200, "text/html", index_html); }
void handleCSS()  { server.send(200, "text/css", style_css); }
void handleJS()   { server.send(200, "application/javascript", script_js); }

void handleConfig() {
  String json = "{ \"sensors\": [";
  for (int i = 0; i < sensorCount; i++) {
    json += "{\"id\":\"" + sensors[i].id + "\",\"label\":\"" + sensors[i].label + "\",\"unit\":\"" + sensors[i].unit + "\"}";
    if (i < sensorCount - 1) json += ",";
  }
  json += "], \"outputs\": [";
  for (int i = 0; i < outputCount; i++) {
    json += "{\"id\":\"" + outputs[i].id + "\",\"label\":\"" + outputs[i].label + "\",\"isPwm\":" + (outputs[i].isPwm ? "true" : "false") + "}";
    if (i < outputCount - 1) json += ",";
  }
  json += "]}";
  server.send(200, "application/json", json);
}

void handleData() {
  String json = "{";
  json += "\"date\": \"" + getDateString() + "\",";
  json += "\"time\": \"" + getTimeString() + "\",";
  json += "\"isRecording\": " + String(isRecording ? "true" : "false") + ",";
  json += "\"sensors\": {";
  for (int i = 0; i < sensorCount; i++) {
    json += "\"" + sensors[i].id + "\": " + String(sensors[i].readFunc());
    if (i < sensorCount - 1) json += ",";
  }
  json += "}}";
  server.send(200, "application/json", json);
}

void handleStart() {
  isRecording = true;
  applyOutputState(true);
  server.send(200, "text/plain", "Started");
}

void handleStop() {
  isRecording = false;
  applyOutputState(false);
  server.send(200, "text/plain", "Stopped");
}

/* ==========================================
*  Setup & Loop
* ========================================== 
*/

void setup() {
  Serial.begin(115200);
  
  // Initialize Pins
  for (int i = 0; i < outputCount; i++) {
    pinMode(outputs[i].pin, OUTPUT);
    digitalWrite(outputs[i].pin, LOW);
  }

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) { delay(500); Serial.print("."); }
  Serial.println("\nConnected! IP: " + WiFi.localIP().toString());

  configTime(gmtOffset_sec, daylightOffset_sec, "pool.ntp.org");

  server.on("/", handleRoot);
  server.on("/style.css", handleCSS);
  server.on("/script.js", handleJS);
  server.on("/api/config", handleConfig);
  server.on("/api/data", handleData);
  server.on("/api/start", handleStart);
  server.on("/api/stop", handleStop);

  server.begin();
}

void loop() { server.handleClient(); }

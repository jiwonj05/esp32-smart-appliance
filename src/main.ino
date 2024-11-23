#include <WiFi.h>
#include <WebServer.h>
#include <NTPClient.h>
#include <WiFiUdo.h>
#include "gpio_init.h"
#include "wifi_init.h"
#include "page_main.h"
#include "time_tracking.h"

const char* ssid = "SHAW-DE8A";
const char* password = "earth2356candid";

#define pin12 12 // Pin for NO 5V relay
#define pin14 14 // Pin for Pushbutton 
#define pin32 32 // ADC1 Pin for V/I measurement
#define pin33 33 // ADC1 Pin for V/I measurement
#define pin34 34 // ADC1 Pin for V/I measurement
#define pin35 35 // ADC1 Pin for V/I measurement
#define pin36 36 // ADC1 Pin for V/I measurement
#define pin39 39 // ADC1 Pin for V/I measurement

// NTP setup
WiFiUDP udp;
NTPClient timeClient(udp, "pool.ntp.org", 0, 60000); // NTP server and update interval (every 60 seconds)

unsigned long applianceOnTimestamp = 0; // Store timestamp when the appliance was turned on
unsigned long applianceOffTimestamp = 0; // Store timestamp when the appliance was turned off
unsigned long applianceDuration = 0; // Store duration when the appliance was on

// const float adcFactor = 0.0008058608059;

float voltage;
float current;

// Set web server port number to 80
WebServer server(80);

volatile bool state = LOW; // Initial state of the relay 
volatile bool buttonPressed = false;
unsigned long lastPressTime = 0;
const unsigned long debounceDelay = 500;

void IRAM_ATTR buttonPress() {
  // Only set buttonPressed if debounce delay has passed
  unsigned long currentTime = millis();
  if ((currentTime - lastPressTime) > debounceDelay) {
    buttonPressed = true;
    lastPressTime = currentTime;
  }
}

void setup() {
    // Set baud rate for serial monitor
    Serial.begin(115200);

    // Initialize relay (pin39) to low 
    gpio_out_low(pin12);

    // Initialize push button
    pinMode(pin14, INPUT_PULLUP);
    
    attachInterrupt(pin14, buttonPress, FALLING); // Interrupt executes when button goes (falls) from high to low

    // Initialize WiFi network
    wifi_startup(ssid, password);

    // Set up routes. These routes are accessed from the browser once the IP address is accessed

    server.on("/", handleRoot); // TODO: Replace with function that will be executed on the server

    server.on("/relay/on", handleOn); // dir to turn on the push button

    server.on("/relay/off", handleOff); // dir to turn off the push button
    
    server.on("/getVoltage", HTTP_GET, handleGetVoltage); // reoute for voltage reading

    server.on("/getStatus", HTTP_GET, handleGetStatus); // Get appliance status
	
	server.on("/getDuration", HTTP_GET, handleGetDuration);  // Get appliance duration

    // Initialize HTTP server
    server.begin();
}

void loop() {
    if (buttonPressed) {
      buttonPressed = false;
      state = !state;
      digitalWrite(pin12, state);
	  // Capture timestamp when the appliance is turned on or off
      if (state == HIGH) {
        applianceOnTimestamp = timeClient.getEpochTime();  // Store the time when the appliance is turned on
	  } else {
        applianceOffTimestamp = timeClient.getEpochTime();  // Store the time when the appliance is turned off
      }
      // Remove Serial.print outputs after testing to reduce flash size 
      Serial.println("Button was pressed");
      Serial.println(state == HIGH ? "Appliance is on" : "Appliance is off"); 
    }
      
    server.handleClient();
    
}

void handleRoot() {
  server.send_P(200, "text/html", PAGE_MAIN);
}

void handleOn() {
  state = HIGH;
  digitalWrite(pin12, state);
  applianceOnTimestamp = timeClient.getEpochTime(); // Capture the off timestamp
  startTimeTracking();  // Start time tracking when appliance is turned on
  server.send_P(200, "text/html", PAGE_MAIN);
}

void handleOff() {
  state = LOW;
  digitalWrite(pin12, state);
  applianceOffTimestamp = timeClient.getEpochTime(); // Capture the off timestamp
  stopTimeTracking();  // Stop time tracking when appliance is turned off
  server.send_P(200, "text/html", PAGE_MAIN); 
}

void handleGetVoltage() {
    // Measure voltage from pin 32
    voltage = analogRead(pin32) * (3.3 / 4095.0);  // Calculate voltage 
    current = analogRead(pin33); 
    String voltageStr = String(voltage, 2);  // Format to 2 decimal places
    server.send(200, "text/plain", voltageStr);  // Send the voltage value to the client
}

// to return the state of the appliance (connected or not)
void handleGetStatus() {
  String status = (state == HIGH) ? "on" : "off";
  server.send(200, "text/plain", status);
}

void handleGetDuration() {
  if (applianceOnTimestamp > 0) {  // Check if appliance has been turned on
    unsigned long currentTime = timeClient.getEpochTime(); // Get current time
    unsigned long duration = currentTime - applianceOnTimestamp;  // Calculate duration in seconds

    // Convert duration to hours, minutes, and seconds
    unsigned int hours = duration / 3600;
    unsigned int minutes = (duration % 3600) / 60;
    unsigned int seconds = duration % 60;

    // Send the formatted duration as response
    String durationStr = String(hours) + " hours " + String(minutes) + " minutes " + String(seconds) + " seconds";
    server.send(200, "text/plain", durationStr);
  } else {
    // If the appliance is off, send "Off" status
    server.send(200, "text/plain", "Off");
  }
}

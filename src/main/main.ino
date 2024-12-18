#include <WiFi.h>
#include <WebServer.h>
#include "gpio_init.h"
#include "wifi_init.h"
#include "page_main.h"

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

    delay(3000);
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

    server.on("/getStatus", HTTP_GET, handleGetStatus);

    // Initialize HTTP server
    server.begin();
}

void loop() {
    if (buttonPressed) {
      buttonPressed = false;
      state = !state;
      digitalWrite(pin12, state);
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
  server.send_P(200, "text/html", PAGE_MAIN);
}

void handleOff() {
  state = LOW;
  digitalWrite(pin12, state);
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



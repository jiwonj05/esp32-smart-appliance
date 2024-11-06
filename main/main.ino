#include <WiFi.h>
#include <WebServer.h>
#include "gpio_init.h"
#include "wifi_init.h"

const char* ssid = "Eriks iPhone";
const char* password = "12345678";

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

    // Initialize HTTP server
    server.begin();
}

void loop() {
    if (buttonPressed) {
    // delay(500); // Accounts for debounce on the push button
      buttonPressed = false;
      state = !state;
      digitalWrite(pin12, state);
      // Remove Serial.print outputs after testing to reduce flash size 
      Serial.println("Button was pressed");
      Serial.println(state == HIGH ? "Appliance is on" : "Appliance is off"); 
    }
      
    server.handleClient();

    // Measure voltage
    // voltage = analogRead(pin32) * adcFactor;
    // Serial.println(analogRead(pin32));
    // voltage = analogRead(pin32) * (3.3/4095);
    

}

void handleRoot() {
  server.send(200, "text/html", SendHTML());
}

void handleOn() {
  state = HIGH;
  digitalWrite(pin12, state);
  server.send(200, "text/html", SendHTML());
}

void handleOff() {
  state = LOW;
  digitalWrite(pin12, state);
  server.send(200, "text/html", SendHTML());
}

String SendHTML(){
    String ptr = "<!DOCTYPE html>\n<html lang=\"en\">\n";
    ptr += "<head>\n<meta charset=\"UTF-8\">\n<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n<title>Appliance Control</title>\n</head>\n";
    ptr += "<body>\n<h1>Appliance Control</h1>\n";
    ptr += "<button id=\"onButton\">Turn On</button>\n";
    ptr += "<button id=\"offButton\">Turn Off</button>\n";
    ptr += "<script>\n";
    ptr += "document.getElementById('onButton').onclick = function() {\n";
    ptr += "fetch('/relay/on') }\n";
    ptr += "document.getElementById('offButton').onclick = function() {\n";
    ptr += "fetch('/relay/off') }\n";
    ptr += "</script>\n";
    ptr += "</body>\n</html>";
    return ptr;    
}



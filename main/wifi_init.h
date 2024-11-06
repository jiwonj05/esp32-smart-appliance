#ifndef WIFI_INIT_H
#define WIFI_INIT_H

#include <WiFi.h>
#include <stdbool.h>
#include <Arduino.h>

void wifi_startup(const char* ssid, const char* password);

#endif // WIFI_INIT_H
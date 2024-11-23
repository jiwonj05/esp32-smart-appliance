#ifndef TIME_TRACKING_H
#define TIME_TRACKING_H

#include <Arduino.h>
#include <time.h>

extern unsigned long applianceOnTimestamp;
extern unsigned long applianceOffTimestamp;

void startTimeTracking() {
  applianceOnTimestamp = time(nullptr);  // Record when appliance is turned on
  Serial.println("Appliance is ON");
}

void stopTimeTracking() {
  applianceOffTimestamp = time(nullptr);  // Record when appliance is turned off
  calculateElapsedTime();
}

void calculateElapsedTime() {
  long elapsedTime = applianceOffTimestamp - applianceOnTimestamp;
  float elapsedHours = elapsedTime / 3600.0;  // Convert seconds to hours

  Serial.print("Elapsed Time: ");
  Serial.print(elapsedHours);
  Serial.println(" hours");

  float costPerKWh = 0.1219;  // Cost per kWh (BC Hydro)
  float cost = powerConsumption * costPerKWh;
  Serial.print("Cost: $");
  Serial.println(cost, 2);  // Print the cost
}

#endif

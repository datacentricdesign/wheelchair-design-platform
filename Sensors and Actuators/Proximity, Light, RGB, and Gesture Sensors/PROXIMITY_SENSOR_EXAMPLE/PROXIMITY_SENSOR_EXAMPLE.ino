// Based on adafruit proximity_sensor example

#include "Adafruit_APDS9960.h"    // Library needed for the Sensor

// Pin that the interrupt is attached to, when object is detected
#define INT_PIN 2

Adafruit_APDS9960 apds;           // Creating our sensor object, so we can use it

void setup() {
  Serial.begin(9600);             // Initializing our Serial port with 9600 baud rate
  pinMode(INT_PIN, INPUT_PULLUP); // Setting up the interrupt pin as an PULL_UP.

  if(!apds.begin()){              // Beginning the work period of the sensor
    Serial.println("failed to initialize device! Please check your wiring.");
  }
  else Serial.println("Proximity Sensor initialized!");

  apds.enableProximity(true);      // Enabling proximity mode

  // Setting the interrupt threshold to fire when proximity reading goes above 100
  apds.setProximityInterruptThreshold(0, 100);

  //  Enabling proximity interrupt
  apds.enableProximityInterrupt();
}

void loop() {

  //print the proximity reading when the interrupt pin goes low
  if(!digitalRead(INT_PIN)){
    Serial.print("Proximity reading (0-255): ");
    Serial.println(apds.readProximity());  // Returns number from 0 - 255, where the higher
                                           // the value the closer an object is to the sensor 
                                           // there is no direct translation to absolut measurements

    // Clear the interrupt event
    apds.clearInterrupt();
  }
}

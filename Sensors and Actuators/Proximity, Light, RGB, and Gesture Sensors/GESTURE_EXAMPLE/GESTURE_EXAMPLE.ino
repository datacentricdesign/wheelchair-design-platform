// Based on adafruit gesture_sensor example

/*  This sketch puts the sensor in gesture mode and decodes gestures
  To use this, first put your hand close to the sensor to enable gesture mode (5cm)
  Then, with your hand about 15cm from the sensor try the gesture  up -> down, 
  down -> up, left -> right, or right -> left direction.
*/

#include "Adafruit_APDS9960.h"  // Library needed for the Sensor

Adafruit_APDS9960 apds;         // Creating our sensor object, so we can use it
uint8_t gesture = 0;            // Variable to hold our gesture

void setup() {
  Serial.begin(9600);           // Initializing our Serial port with 9600 baud rate
  
  if(!apds.begin()){            // Begining the work period of the sensor
    Serial.println("Failed to initialize Sensor! Please check your wiring.");
  }
  else Serial.println("Gesture Sensor initialized!");

  //gesture mode will be entered once proximity mode senses something close
  apds.enableProximity(true);   // Enabling proximity detection
  apds.enableGesture(true);     // Enabling Gesture detection
}

// the loop function runs over and over again forever
void loop() {
  //read a gesture from the device
    gesture = apds.readGesture(); // Read gesture into the variable

    // Processing captured gesture, if any.
    if(gesture == APDS9960_DOWN) Serial.println("⬇");
    if(gesture == APDS9960_UP) Serial.println("⬆");
    if(gesture == APDS9960_LEFT) Serial.println("⬅");
    if(gesture == APDS9960_RIGHT) Serial.println("➡");
}

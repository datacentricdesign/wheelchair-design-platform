// based on Adafruit color_sensor example

#include "Adafruit_APDS9960.h"  // Library needed for the Sensor

Adafruit_APDS9960 apds;         // Creating our sensor object, so we can use it


uint16_t red, green, blue, clr;            // Global variables to store our color data
                                           // Each channel is 16bit

void setup() {
  
  Serial.begin(9600);           // Initializing our Serial port with 9600 baud rate

  if(!apds.begin()){            // Beginning the work period of the sensor
    Serial.println("failed to initialize device! Please check your wiring.");
  }
  else Serial.println("RGBC Sensor initialized!");

  // enable color sensing mode
  apds.enableColor(true);
}

void loop() {

  
  // we need to wait for the color data to be ready
  while(!apds.colorDataReady()){
    delay(5);
  }

  // retrieving color data, careful, we're using the & operator, we dont pass the 
  // variables to the function, but the ADDRESSES of the variables (& operator on a variable)
  apds.getColorData(&red, &green, &blue, &clr);


  // Printing our data 
  Serial.print("Red: ");
  Serial.println(red);
  
  Serial.print("Green: ");
  Serial.println(green);
  
  Serial.print("Blue: ");
  Serial.println(blue);

  // Clear is used for ambient light sensing
  Serial.print("Clear: ");
  Serial.println(clr);
  Serial.println();
  
  delay(500);                     // New sample every half a second
}

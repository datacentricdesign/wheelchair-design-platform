#include <Adafruit_NeoPixel.h> // Necessary Library include


#define LED_PIN 6 // Defining the pin of the arduino that sends the data stream.

Adafruit_NeoPixel LED_controller = Adafruit_NeoPixel( 1, LED_PIN, NEO_RGB + NEO_KHZ800); 
// This is a class that will control your NeoPixel LEDs
//( NUMBER OF LEDS Connected, PIN NUMBER, LED TYPE, IN OUR CASE NEO_RGB + NEO_KHZ800),
// NEO_KHZ800 just mentions the 800 KHz support, no need to change for most cases.
uint8_t R = 0, G = 0, B = 0; // Unsigned integer with 8 bits
uint32_t counter = 0; // 32 bits unsigned integer, we only need 24 to go through all the colors

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); //Set serial to 9600 baud
  LED_controller.begin(); // We're starting up the library

  LED_controller.setPixelColor( 0, 0xFFFFFF);
}

void loop() {
  //( led number, R, G , and B values, for the color function)
  // .setPixelColor takes the led and the color. you can also use .color, you can find an example commented
  // .Color() takes values from 0, 0, 0, to 255, 255, 255

  // This Example goes through some of all the possible RGB Values!
  // careful , theset pixel color uses INDICES for the LEDS , so it starts at 0
//  LED_controller.setPixelColor( 0, 0xFFFFFF);
  // another example of setting a particular color
  // LED_controller.setPixelColor( 1, LED_controller.Color( 0, 150, 0 ) ); // Green!
//  Serial.print(" RGB Color ( 0x000000 to 0xFFFFFF):  ");
//  Serial.println(counter, HEX);

  LED_controller.show(); // Sending updated pixel color to the hardware

//  if(counter >= 0xFFFFFF) counter = 0; // once we go over the the maximum value 255 255 255, we go back to 0 
//  counter+= 10000;
//  counter&= 0xFFFFFF; // bit mask to only keep the first 24bits

}

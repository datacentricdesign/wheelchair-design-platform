/* FSR testing sketch.

Connect one end of FSR to 5V, the other end to Analog 0.
Then connect one end of a 10K resistor from Analog 0 to ground
Connect LED from pin 11 through a resistor to ground

For more information see www.ladyada.net/learn/sensors/fsr.html */

#define NUMBER_FSR 9
int fsrPins[NUMBER_FSR];
int fsrReading[NUMBER_FSR];

int LEDpin = 12;      // connect Red LED to pin 11 (PWM pin)
int LEDbrightness;

void setup(void) {
  Serial.begin(9600);   // We'll send debugging information via the Serial monitor
  pinMode(LEDpin, OUTPUT);

  for (int i=0; i < NUMBER_FSR; i++){//Assign the amount of FSR sensors to its array and stablishes its pins as INPUT
    fsrPins[i] = i; //fsrPins = i
    pinMode(i, INPUT);
  }

}

void loop(void) {
  for (int i=0; i<NUMBER_FSR; i++){
    fsrReading[i]=analogRead(fsrPins[i]);

    Serial.print("FSR");
    Serial.print(i);
    Serial.print("=");
    Serial.print(fsrReading[i]);
    if (i<=NUMBER_FSR-1){
    Serial.print(",");
    Serial.print(" ,");
    }
  }
  Serial.println("");


  // we'll need to change the range from the analog reading (0-1023) down to the range
  // used by analogWrite (0-255) with map!
  LEDbrightness = map(fsrReading[0], 0, 1023, 0, 255);
  // LED gets brighter the harder you press
  analogWrite(LEDpin, LEDbrightness);

  delay(100);
}

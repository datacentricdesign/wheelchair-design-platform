/* FSR testing sketch.

Connect one end of FSR to 5V, the other end to Analog 0.
Then connect one end of a 10K resistor from Analog 0 to ground
Connect LED from pin 11 through a resistor to ground

For more information see www.ladyada.net/learn/sensors/fsr.html */

#define NUMBER_FSR 9
int fsrPins[NUMBER_FSR];
int fsrReading[NUMBER_FSR];

int inPinB = 2;         // the number of the input pin
int outPinL = 13;       // the number of the output pin
int state = LOW;      // the current state of the output pin
int reading;           // the current reading from the input pin
int previous = HIGH;    // the previous reading from the input pin


void setup(void) {


  Serial.begin(115200);   // We'll send debugging information via the Serial monitor

  for (int i=0; i < NUMBER_FSR; i++){//Assign the amount of FSR sensors to its array and stablishes its pins as INPUT
    fsrPins[i] = i; //fsrPins = i
    pinMode(i, INPUT);
  }

  pinMode(inPinB, INPUT);
  pinMode(outPinL, OUTPUT);

}

void loop(void) {
  
  reading = digitalRead(inPinB);

  if (reading == HIGH)


  if (reading == HIGH && previous == LOW) {
    if (state == HIGH)
      state = LOW;
    else
      state = HIGH;
   
  }
  
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

  Serial.print("Button");
  Serial.print("=");
  Serial.println(state);

  digitalWrite(outPinL, state);

  previous = reading;

  delay(100);
}

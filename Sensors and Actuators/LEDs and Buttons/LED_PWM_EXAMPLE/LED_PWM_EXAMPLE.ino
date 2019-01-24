
#define LED_PIN 2 // Careful, here we have to use a pin that can be used for pwm.

 int i = 0; // Our counter for PWM, we declare it globally
// so it lasts for the duration of the entire program
boolean direction_down = 0; // boolean so we know when to start to invert direction of the counter
void setup() {
  // put your setup code here, to run once:
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(9600); // setting baud speed for Serial (a baud is a pulse)
  Serial.println("Lets start our pulsing LED example!");
}

void loop() {

  // PWM takes values from 0 to 255, in our case, we want to make
  // a pulse effect, so we detect out of bounds behaviour and just
  // invert the direction of our counter, thus brightening and dimming.
  // We'll be using the upper bound of 80 since, the change in brightness
  // is not as noticeable from 80 to 255 as in the lower numbers (0 to 80)
  // so we're continuously going from 0 - 80 - 0 ... etc.
  
  if( i == 80)
    direction_down = true;
  else if( i == 0)
    direction_down = false; 
  
  if(direction_down) 
    i--;
  else
    i++;

    Serial.print("Led intensity (0 to 255):  ");
    Serial.println(i);
    analogWrite(LED_PIN, i);  
}

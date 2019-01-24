// Lets make our vibration example, using PWM! 
#define VIB_PIN 10 // Careful, here we have to use a pin that can be used for pwm.

int i = 127;       // Our counter for PWM, we declare it globally, 
                   // So it lasts for the duration of the entire program. 
                   // It starts in 127 since the vibration motor starts 
                   // working from around 2 volts (so we start at 2.5v)

void setup() {
  // put your setup code here, to run once:
  pinMode(VIB_PIN, OUTPUT);
  Serial.begin(9600); // setting baud speed for Serial (a baud is a pulse)
  Serial.println("Lets start our pulsing vibration example!");
}

void loop() {

  // PWM takes values from 0 to 255, in our case, we want to make
  // a pulse effect, so we detect out of bounds behaviour and just
  // invert the direction of our counter, thus brightening and dimming.
  // We'll be using the upper bound of 255 since, the change in brightness
  // is not as noticeable from 80 to 255 as in the lower numbers (0 to 80)
  // so we're continuously going from 0 - 80 - 0 ... etc.
  
  if( i == 255)
    i = 127;
  
    i++;
    
    Serial.print("Vibration intensity (127 to 255):  ");
    Serial.println(i);
    analogWrite(VIB_PIN, i);
    delay(i*10); // in each step of pwm, we vibrate for i * 0.01 seconds  
}


33 lines (23 sloc) 1.06 KB
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
  // a pulse effect, so we detect out of bounds behaviour and go to 127
 
  i+=10;  // incrementing the power of the vibration motor
  
  if( i > 255) 
    i = 127;


    Serial.print("Vibration intensity (127 to 255):  ");
    Serial.println(i);
    analogWrite(VIB_PIN, i);
    delay(i*10); // in each step of pwm, we vibrate for i * 0.01 seconds 
  
}
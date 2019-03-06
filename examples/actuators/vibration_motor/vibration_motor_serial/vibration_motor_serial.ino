// In this example, the vibration motor is increasing speed up to max speed, then
// decreases to half speed before starting increasing  again

// Lets make our vibration example, using PWM!
#define VIB_PIN 10 // Careful, here we have to use a pin that can be used for pwm.

int i = 127;       // Our counter for PWM, we declare it globally, 
                   // So it lasts for the duration of the entire program. 
                   // It starts in 127 since the vibration motor starts 
                   // working from around 2 volts (so we start at 2.5v)

bool increase = true;
bool vibration_enabled = false;

void setup() {
  // put your setup code here, to run once:
  pinMode(VIB_PIN, OUTPUT);
  Serial.begin(9600);
}

void vibration_pattern() {
  if (increase) {
    i+=10;  // incrementing the power of the vibration motor
  } else {
    i-=10;
  }

  if ( i > 255) {
    increase = false;
  } else if ( i < 127) {
    increase = true;
  }
}

void loop() {
  char command = Serial.read();
  if (command == '1') {
    Serial.println("Turning on Vibration...");
    vibration_enabled = true;
  } else if (command == '0') {
    Serial.println("Turning off Vibration...");
    vibration_enabled = false;
    analogWrite(VIB_PIN, 0);
  }
  if (vibration_enabled) {
    vibration_pattern();
    analogWrite(VIB_PIN, i);
  }
  delay(50);
}

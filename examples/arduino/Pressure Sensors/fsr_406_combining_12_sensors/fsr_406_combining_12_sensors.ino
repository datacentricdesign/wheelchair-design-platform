// Setting up pins to receive voltage
static const uint8_t FSR[] = {A0,A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11};

// 1 sample / 100 millisecond (10Hz)
static const int SAMPLING_RATE = 100;

// Setting the minimum deviation between the measurements (0 by default)
// up to 512 (although that is pretty useless). Since there's a bit of a drift
// in the values if you put the same pressure over a certain period, we ignore
// a divergence of around 1 percent around the previous value.
static const int DEVIATION = 10;

// Property id
static const String PROPERTY_ID = "fsr-1ebb";

// Current value from sensor (read from analog port)
int value;
// Array of previous values (raw)
int prev_values[sizeof(FSR)];

// Converted into Voltage
double voltage_value;
// Array of current values (converted into Newton)
double newton_values[sizeof(FSR)];
// Does one of the current 12 values deviate from its respective previous value?
bool is_deviating = false;

// This is based on a 3.3kΩ resistor being used, with weights on the center
// that do not linger more than 10 seconds. For situations that deviate from
// this, accuracy can be lost.
double convert_to_newtons( double voltage) {
   double a = 0.01419 ;
   double b = 0.9523;
   double c = -0.01461;
   double d = -2.231;

   // The result of the fit is in KgF to convert into newton we simply
   // multiply by 9.81, if you want data in KgF, remove the final multiplication!
   return( (a*exp(b*voltage) + c*exp(d* voltage)) * 9.81 );

}

// Send data over serial
void push_data(double values[]) {
    Serial.print(PROPERTY_ID);
    for (int i = 0; i<sizeof(FSR); i++) {
      Serial.print(",");
      Serial.print(values[i]);
    }
    Serial.println();
}

void setup() {
  // Init
  for (int i = 0; i<sizeof(FSR); i++) {
    prev_values[i] = -10000;
    digitalRead(FSR[i]);
  }
  // Serial Connection setup
  Serial.begin(9600);
}

void loop() {
  // Here we can set the sampling rate, right now 10 Hz
  delay(SAMPLING_RATE);

  for (int i = 0; i<sizeof(FSR); i++) {
    // Reading our analog voltage, careful we only have 10 bit resolution so each
    // measurement step is only 5V ÷ 1024, so our result will be 0 - 1023
    value = analogRead(FSR[i]);

    if( value < (prev_values[i] - DEVIATION)
        || value > (prev_values[i] + DEVIATION) ) {
      // if value is outside the range of [ previous - σ , previous + σ],
      // flag that at least 1 fsr is deviating (if value is relatively the same)
      // this will help with having data ocuppy your buffer that is not a significant deviation.
      is_deviating = true;
    }

    // Converting to voltage [ 0, 5] v.
    voltage_value = double((value*5)) / 1023;

    // getting actual force value (careful using this, accuracy may not be ideal)
    // sensitivity after 1Kgf and before 0.06kgf is limited, you can lower the
    // deviation for some improvements
    newton_values[i] = convert_to_newtons(voltage_value);

    prev_values[i] = value;
  }

  // If at least 1 fsr is deviating, push the data
  if (is_deviating) {
    push_data(newton_values);
    is_deviating = false;
  }
}

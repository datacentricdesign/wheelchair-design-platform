#define PRESSURE_PIN  A0        // Setting up pin to receive voltage

int value, prev_value = - 10000;     // int values (read from analog port, both the current and the previous)
int deviation = 0;              // setting the minimum deviation between the measurements (0 by default)
                                // up to 512 (although that is pretty useless)
double voltage_value, newton_value;           // Converted to Voltage 

void setup() {
  // Serial Connection setup
  Serial.begin(9600);
  Serial.println("Lets begin our pressure example!");
  pinMode(PRESSURE_PIN, INPUT); // setting pinmode to read analog value 

  deviation = 10;  // since there's a bit of a drift in the values if you put the same pressure over a certain period
                   // we ignore a divergence of around 1 percent around the previous value. 
}

// This is based on a 3.3kΩ resistor being used, with weights on the center that do not linger more than 10 seconds
// For situations that deviate from this, accuracy can be lost.
double convert_to_newtons( double voltage) 
{
  /* General fitting model Exp2:
     f(x) = a*exp(b*x) + c*exp(d*x)
     Coefficients (with 95% confidence bounds):
       a =     0.01419  (0.01163, 0.01676)
       b =      0.9523  (0.8922, 1.012)
       c =    -0.01461  (-0.02317, -0.006043)
       d =      -2.231  (-6.605, 2.142)

       Goodness of fit:
       SSE: 7.906e-06
       R-square: 0.9999
       Adjusted R-square: 0.9997
       RMSE: 0.001988
   */
   double a = 0.01419 ;
   double b = 0.9523;
   double c = -0.01461;
   double d = -2.231;
    
  return( (a*exp(b*voltage) + c*exp(d* voltage)) * 9.81 ); // the result of the fit is in KgF to convert to newton we simply
                                                      // multiply by 9.81, if you want data in KgF, remove the final multiplication!
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(100);                   // Here we can set the sampling rate, right now 10 Hz
  value = analogRead(PRESSURE_PIN); // reading our analog voltage, careful we only have 10 bit resolution so each
                                    // measurement step is only 5V ÷ 1024, so our result will be 0 - 1023

  // if value is within the range of [ previous - σ , previous + σ], ignore it (if value is relatively the same)
  // this will help with having data ocuppy your buffer that is not a significant deviation.
  if( value >= (prev_value - deviation) && value <= (prev_value + deviation) )
    return;
   
  voltage_value = double((value*5)) / 1023; // converting to voltage [ 0, 5] v.
  newton_value = convert_to_newtons(voltage_value); // getting actual force value (careful using this, accuracy may not be ideal) 
                                                    // sensitivity after 1Kgf and before 0.06kgf is limited, you can lower the deviation
                                                    // for some improvements
  Serial.print("Pressure: ");
  Serial.print(value);
  Serial.print(" (0 - 1023) steps,  ");
  Serial.print(voltage_value);
  Serial.print(" (v),  ");
  Serial.print(newton_value);
  Serial.println(" N.");

  prev_value = value;
  
}

#define IR_PIN  A0                        // Setting up pin to receive voltage from IR

int value, prev_value = - 10000;          // int values (read from analog port, both the current and the previous)
int deviation = 0;                        // setting the minimum deviation between the measurements (0 by default)
                                          // up to 512 (although that is pretty useless)
double voltage_value, distance_value;    // Converted to Voltage 

void setup() {
  // Serial Connection setup
  Serial.begin(9600);
  Serial.println("Lets begin our pressure example!");
  pinMode(IR_PIN, INPUT); // setting pinmode to read analog value 

  deviation = 10;  // since there's a bit of a drift in the values if you put the same object over a certain period
                   // we ignore a divergence of around 1 percent around the previous value. 
}

// Function to convert this voltage into a distance, be careful this is the result of experimental results, so only an estimate.
// Also this is based on white paper, so lighting properties of the object also change these measurements.
double convert_to_distance( double voltage) 
{
    /*General model Exp2:
     f(x) = a*exp(b*x) + c*exp(d*x)
     Coefficients (with 95% confidence bounds):
       a =       256.1  (198.7, 313.5)
       b =      -2.128  (-3.302, -0.9536)
       c =       51.24  (-48.55, 151)
       d =     -0.3724  (-1.159, 0.414)

      Goodness of fit:
      SSE: 88.64
      R-square: 0.9961
      Adjusted R-square: 0.9949
      RMSE: 2.977
  */

  double a =  256.1; 
  double b = -2.128; 
  double c =  51.24;
  double d = -0.3724;   
  
  return(a*exp(b*voltage) + c*exp(d*voltage));
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(100);                   // Here we can set the sampling rate, right now 10 Hz
  value = analogRead(IR_PIN);  // reading our analog voltage, careful we only have 10 bit resolution so each
                                    // measurement step is only 5V ÷ 1024, so our result will be 0 - 1023

  // if value is within the range of [ previous - σ , previous + σ], ignore it (if value is relatively the same)
  // this will help with having data ocuppy your buffer that is not a significant deviation.
  if( value >= (prev_value - deviation) && value <= (prev_value + deviation) )
    return;
   
  voltage_value = double((value*5)) / 1023; // converting to voltage [ 0, 5] v.
  distance_value = convert_to_distance(voltage_value); // getting actual distance value(cm) (careful using this, accuracy may not be ideal) 

  if(distance_value  <= 15 || distance_value >= 150 ) // We will ignore values outside the range of measurement
    return; 
    
  Serial.print("Distance: ");
  Serial.print(value);
  Serial.print(" (0 - 1023) steps,  ");
  Serial.print(voltage_value);
  Serial.print(" (v),  ");
  Serial.print(distance_value);
  Serial.println(" cm.");

  prev_value = value;
  
}

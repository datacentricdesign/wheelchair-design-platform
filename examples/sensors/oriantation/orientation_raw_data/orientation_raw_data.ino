/* Based on Adafruit sensorapi and rawdata examples
   This driver uses the Adafruit unified sensor library (Adafruit_Sensor),
   which provides a common 'type' for sensor data and some helper functions
   Since this example uses raw data, we only use the USL . for debugging purposes.


   You should also assign a unique ID to this sensor for use with
   the Adafruit Sensor API so that you can identify this particular
   sensor in any data logs, etc.  To assign a unique ID, simply
   provide an appropriate value in the constructor below (12345
   is used by default in this example).
*/

#include <Wire.h>                 // Library used for communication with I2C
#include <Adafruit_Sensor.h>      // Adafruit unified sensor library
#include <Adafruit_BNO055.h>      // Adafruit library for this breakout
#include <utility/imumaths.h>     // Utility to define vector, matrices and quaternions,
                                  // And their helper functions


// Here, we set the delay, in ms,  between fresh samples (the sample rate)
#define DELAY_MS 1000

// Creating our sensor object to handle the sensor, with initialization 12345
Adafruit_BNO055 bno = Adafruit_BNO055(12345);

// Here we create our raw data variables
imu::Vector<3> euler_vector;            // for reading euler vector values
                                        // (values in Euler angles or 'degrees', from 0..359)
imu::Vector<3> magnetometer_vector;     // for reading magnetometer vector values
                                        // (values in uT, micro Tesla)
imu::Vector<3> gyroscope_vector;        // for reading gyroscope vector values
                                        // (values in rps, radians per second)
imu::Vector<3> accelerometer_vector;    // for reading accelerometer vector values
                                        // (values in m/s^2)
imu::Vector<3> linear_accel_vector;     // for reading linear acceleration vector values
                                        // (values in m/s^2)
imu::Vector<3> gravity_vector;          // for reading gravity vector values
                                        // (values in m/s^2)


imu::Quaternion quaternion;             // for reading quaternion values
int8_t temperature;                     // for reading temperature values (8 bytes)



// Arduino setup function (automatically called at startup)
void setup(void)
{
  Serial.begin(9600);
  Serial.println("Lets begin our Orientation Sensor Test"); Serial.println("");

  /* Initialise the sensor */
  if(!bno.begin())
  {
    /* Problem detecting the BNO055 ... wiring error */
    Serial.print("No BNO055 is detected ... Check your wiring or I2C ADDR!");
    while(1);
  }

  delay(1000);

  /* Display some basic information on this sensor */
  displaySensorDetails();

  /* Optional: Display current status */
  displaySensorStatus();

  bno.setExtCrystalUse(true);       // by doing this we are using an external 32khz
                                    // crystal (the one on the development board)
}

// Arduino loop function, called once 'setup' is complete 

void loop(void)
{
  /* Other parameters of getVector function
      VECTOR_MAGNETOMETER (values in uT, micro Teslas)
      VECTOR_GYROSCOPE (values in rps, radians per second)
      VECTOR_EULER (values in Euler angles or 'degrees', from 0..359)
      VECTOR_ACCELEROMETER (values in m/s^2)
      VECTOR_LINEARACCEL (values in m/s^2)
      VECTOR_GRAVITY (values in m/s^2)
   */
  
  // Extracting raw data vectors 
  euler_vector= bno.getVector(Adafruit_BNO055::VECTOR_EULER);            
  magnetometer_vector = bno.getVector(Adafruit_BNO055::VECTOR_MAGNETOMETER);     
  gyroscope_vector = bno.getVector(Adafruit_BNO055:: VECTOR_GYROSCOPE);;        
  accelerometer_vector = bno.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);   
  linear_accel_vector = bno.getVector(Adafruit_BNO055::VECTOR_LINEARACCEL);
  gravity_vector = bno.getVector(Adafruit_BNO055::VECTOR_GRAVITY);

  // Extracting other raw data
  quaternion = bno.getQuat();    // get quaternion helper function         
  temperature = bno.getTemp();   // get temperature helper function


  /* Display the floating point data */
  
  Serial.println("euler:");
  Serial.println("");
  Serial.print("X: ");
  Serial.print(euler_vector.x());
  Serial.print(" Y: ");
  Serial.print(euler_vector.y());
  Serial.print(" Z: ");
  Serial.print(euler_vector.z());
  Serial.println("");
  Serial.println("");

  delay(50);

  Serial.println("magnetometer:");
  Serial.println("");
  Serial.print("X: ");
  Serial.print(magnetometer_vector.x());
  Serial.print(" Y: ");
  Serial.print(magnetometer_vector.y());
  Serial.print(" Z: ");
  Serial.print(magnetometer_vector.z());
  Serial.println("");
  Serial.println("");

  delay(50);

  Serial.println("gyroscope:");
  Serial.println("");
  Serial.print("X: ");
  Serial.print(gyroscope_vector.x());
  Serial.print(" Y: ");
  Serial.print(gyroscope_vector.y());
  Serial.print(" Z: ");
  Serial.print(gyroscope_vector.z());
  Serial.println("");
  Serial.println("");
  delay(50);
  
  Serial.println("accelerometer:");
  Serial.println("");
  Serial.print("X: ");
  Serial.print(accelerometer_vector.x());
  Serial.print(" Y: ");
  Serial.print(accelerometer_vector.y());
  Serial.print(" Z: ");
  Serial.print(accelerometer_vector.z());
  Serial.println("");
  Serial.println("");
  delay(50);
  
  Serial.println("linear acceleration:");
  Serial.println("");
  Serial.print("X: ");
  Serial.print(linear_accel_vector.x());
  Serial.print(" Y: ");
  Serial.print(linear_accel_vector.y());
  Serial.print(" Z: ");
  Serial.print(linear_accel_vector.z());
  Serial.println("");
  Serial.println("");
  delay(50);
  
  Serial.println("gravity:");
  Serial.println("");
  Serial.print("X: ");
  Serial.print(gravity_vector.x());
  Serial.print(" Y: ");
  Serial.print(gravity_vector.y());
  Serial.print(" Z: ");
  Serial.print(gravity_vector.z());
  Serial.println("");
  Serial.println("");
  delay(50);

  /* Display the quaternion data, 4 decimal places */
  Serial.println("Quaternion:");
  Serial.print("qW: ");
  Serial.print(quaternion.w(), 4);
  Serial.print(" qX: ");
  Serial.print(quaternion.y(), 4);
  Serial.print(" qY: ");
  Serial.print(quaternion.x(), 4);
  Serial.print(" qZ: ");
  Serial.print(quaternion.z(), 4);
  Serial.println("");
  Serial.println("");
  delay(50);

  Serial.print("Current Temperature: ");
  Serial.println("");
  Serial.print(temperature);
  Serial.println(" C");
  Serial.println("");
  Serial.println("");
  
  /* Optional: Display calibration status */
  // displayCalStatus();

  /* Optional: Display sensor status (for debug purposes only) */
  //displaySensorStatus();

  /* New line for the next sample */
  Serial.println("");

  /* Wait the specified delay before requesting nex data */
  delay(DELAY_MS) ;
}

//  Displays some basic information on this sensor from the unified
//  sensor API sensor_t type (see Adafruit_Sensor for more information)
void displaySensorDetails(void)
{
  sensor_t sensor;
  bno.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" xxx");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" xxx");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" xxx");
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);
}


// Display some basic info about the sensor status
void displaySensorStatus(void)
{
  /* Get the system status values (mostly for debugging purposes) */
  uint8_t system_status, self_test_results, system_error;
  system_status = self_test_results = system_error = 0;
  bno.getSystemStatus(&system_status, &self_test_results, &system_error);

  /* Display the results in the Serial Monitor */
  Serial.println("");
  Serial.print("System Status: 0x");
  Serial.println(system_status, HEX);
  Serial.print("Self Test:     0x");
  Serial.println(self_test_results, HEX);
  Serial.print("System Error:  0x");
  Serial.println(system_error, HEX);
  Serial.println("");
  delay(500);
}


// Display sensor calibration status
void displayCalStatus(void)
{
  /* Get the four calibration values (0..3) */
  /* Any sensor data reporting 0 should be ignored, */
  /* 3 means 'fully calibrated" */
  uint8_t system, gyro, accel, mag;     // variable for gyroscope, accelerometer
                                        // and magnetometer
  system = gyro = accel = mag = 0;
  bno.getCalibration(&system, &gyro, &accel, &mag);

  /* The data should be ignored until the system calibration is > 0 */
  Serial.print("\t");
  if (!system)
  {
    Serial.print("! ");
  }

  /* Display the individual values */
  Serial.print("Sys:");
  Serial.print(system, DEC);        // Printing decimal values  
  Serial.print(" G:");
  Serial.print(gyro, DEC);
  Serial.print(" A:");
  Serial.print(accel, DEC);
  Serial.print(" M:");
  Serial.print(mag, DEC);
}

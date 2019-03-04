/* Based on Adafruit sensorapi example
   This driver uses the Adafruit unified sensor library (Adafruit_Sensor),
   which provides a common 'type' for sensor data and some helper functions.


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

// structure to store total rotations since IMU  initialized, forward and reverse
// initialized with a global variable global_rotations, this variable stores rotations
// on a particular axis, in both directions, since startup
struct Rotations {
  float forward_rotations = 0;
  float reverse_rotations = 0;
} global_rotations;


bool not_first_loop = false; // Boolean variable to stop logging of first loop
float previous_axis_value = 666;  // Initial value so we don't account for it
// Creating our sensor object to handle the sensor, with initialization 12345
Adafruit_BNO055 bno = Adafruit_BNO055(12345);

// Function to compute rotation on a given axis
// takes the axis that you want to use and variable it can change 
// CAREFUL YOU MUST PASS ROTATIONS AS A POINTER
// (float axis, Rotations * rotations)
bool compute_rotations(float axis, Rotations * rotations) {
  static float initial_axis_value = axis;
  // variable to store initial axis value in compute rotations - declared static so that it stores
  // this value in between function calls, but no other functions can change its value
  //Variables declared as static will only be created and initialized the first time a function is called
  
  float offset_rot = (axis-previous_axis_value) / 360; // offset since previous measurement, in rotations

  // so we do not account for anything in the setup phase
  if (previous_axis_value == 666) {
    offset_rot = 0;
  }

  if(offset_rot >= 0) {
    (rotations->forward_rotations) += offset_rot;
  } else {
    (rotations->reverse_rotations) += offset_rot;
  }
  
  // place previous axis value
  previous_axis_value = axis;
   
  return(true); // returns true by default, do not remove, as it helps with the initial setup.
}

// Arduino setup function (automatically called at startup)
void setup(void) {
  Serial.begin(9600);
  Serial.println("Lets begin our Orientation Sensor Test"); Serial.println("");

  // Initialise the sensor
  if(!bno.begin()) {
    // Problem detecting the BNO055 ... wiring error
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
void loop(void) {
  /* Get a new sensor event */
  sensors_event_t event;
  bno.getEvent(&event);
  
  /* Display the floating point data */
  Serial.print("X: ");
  Serial.print(event.orientation.x, 4);   // 4 decimal places
  Serial.print("\tY: ");
  Serial.print(event.orientation.y, 4);
  Serial.print("\tZ: ");
  Serial.print(event.orientation.z, 4);

  // if this is the first loop iteration, ignore position data (always zero)  
  //if its second loop iteration set the starting position for your axis 
  // if its another iteration, just continue computing the rotation data 

  float axis_value = event.orientation.x;   // replace this with whatever axis you're tracking
  not_first_loop = (not_first_loop)?compute_rotations(axis_value, &global_rotations) : true;

  Serial.println("");
  Serial.println("==============");
  Serial.println("Total Rotations in selected axis");
  Serial.print("Positive: ");
  Serial.println(global_rotations.forward_rotations);
  Serial.print("Negative: ");
  Serial.println(-global_rotations.reverse_rotations);
  Serial.println("==============");

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
void displaySensorDetails(void) {
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
void displaySensorStatus(void) {
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
void displayCalStatus(void) {
  /* Get the four calibration values (0..3) */
  /* Any sensor data reporting 0 should be ignored, */
  /* 3 means 'fully calibrated" */
  uint8_t system, gyro, accel, mag;     // variable for gyroscope, accelerometer
                                        // and magnetometer
  system = gyro = accel = mag = 0;
  bno.getCalibration(&system, &gyro, &accel, &mag);

  /* The data should be ignored until the system calibration is > 0 */
  Serial.print("\t");
  if (!system) {
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

// Based on Adafruit getcurrent example

#include <Wire.h>     // Library used for communication with I2C/ TWI devices
#include <Adafruit_INA219.h> // Library used for the INA219 sensor

Adafruit_INA219 ina219; // Initializing Object that will manipulate the sensor
// This will, optionally take an address, in case more than one sensor is used.

// The variables that will hold our data
float shuntvoltage = 0; 
float busvoltage = 0;   
float current_mA = 0;
float loadvoltage = 0;
float power_mW = 0;
  
void setup(void) 
{
  Serial.begin(9600); // Startup of Serial with 9600 baud rate;
  
  while (!Serial) {
      // will pause the board, until the console is opened;
      delay(1);
  }
    
  Serial.println("Lets begin with our sensor example, to measure voltage and current!");
  
  // Initialize the INA219.
  // By default the initialization will use the largest range (32V, 2A).  However
  // you can call a setCalibration function to change this range.
  ina219.begin(); 
  // To use a slightly lower 32V, 1A range (higher precision on amps):
  //ina219.setCalibration_32V_1A();
  // Or to use a lower 16V, 400mA range (higher precision on volts and amps):
  //ina219.setCalibration_16V_400mA();

}

void loop(void) 
{


  shuntvoltage = ina219.getShuntVoltage_mV(); // Reading Shunt Voltage
  busvoltage = ina219.getBusVoltage_V();      // Reading Bus Voltage
  current_mA = ina219.getCurrent_mA();        // Read Current
  power_mW = ina219.getPower_mW();            // Read Power
  loadvoltage = busvoltage + (shuntvoltage / 1000); // Read Voltage of load ( in v)
  
  Serial.print("Bus Voltage:   "); Serial.print(busvoltage); Serial.println(" V");
  Serial.print("Shunt Voltage: "); Serial.print(shuntvoltage); Serial.println(" mV");
  Serial.print("Load Voltage:  "); Serial.print(loadvoltage); Serial.println(" V");
  Serial.print("Current:       "); Serial.print(current_mA); Serial.println(" mA");
  Serial.print("Power:         "); Serial.print(power_mW); Serial.println(" mW");
  Serial.println("");

  delay(2000); // We will update our samples every 2 seconds.
}

/*********************************************************************
 This is an example for our nRF51822 based Bluefruit LE modules

 Pick one up today in the adafruit shop!

 Adafruit invests time and resources providing this open source code,
 please support Adafruit and open-source hardware by purchasing
 products from Adafruit!

 MIT license, check LICENSE for more information
 All text above, and the splash screen below must be included in
 any redistribution
*********************************************************************/

/*
    Please note the long strings of data sent mean the *RTS* pin is
    required with UART to slow down data sent to the Bluefruit LE!
*/
#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h"
#include "Adafruit_BluefruitLE_UART.h"

#include "BluefruitConfig.h"

#if SOFTWARE_SERIAL_AVAILABLE
  #include <SoftwareSerial.h>
#endif

#include "BluefruitConfig.h"

// LED error flag
#define LED_PIN 2

// Create the Bluefruit object for Feather 32u4
Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);

// BNO settings
#define BNO055_SAMPLERATE_DELAY_MS (200)
Adafruit_BNO055 bno = Adafruit_BNO055(55);

// GATT service information
int32_t imuServiceId;
int32_t orientationCharId;

// A small helper
void error(const __FlashStringHelper*err) {
  if (Serial.available()) {
    Serial.println(err);
  }
  // In any case, turn on the LED to signal the error
  analogWrite(LED_PIN, HIGH);
  while (1);
}

// Initializes BNO055 sensor
void initSensor(void) {
  if(!bno.begin()) {
    error(F("No BNO055 detected. Check your wiring or I2C ADDR!"));
  }
  delay(1000);
  bno.setExtCrystalUse(true);
}

// Sets up the HW an the BLE module (this function is called
// automatically on startup)
void setup(void) {
  delay(500);
  boolean success;

  // Set LED error flag

  pinMode(LED_PIN, OUTPUT);
  analogWrite(LED_PIN, LOW);
  Serial.begin(115200);

  // Initialise the module
  if ( !ble.begin(VERBOSE_MODE) ) {
    error(F("Couldn't find Bluefruit, make sure it's in CoMmanD mode & check wiring."));
  }

   // Setup the BNO055 sensor
  initSensor();

  // Perform a factory reset to make sure everything is in a known state
  if (! ble.factoryReset() ){
       error(F("Couldn't factory reset."));
  }

  // Disable command echo from Bluefruit
  ble.echo(false);

  // Print Bluefruit information
  ble.info();
  ble.verbose(true);

  // Change the device name to fit its purpose
  if (! ble.sendCommandCheckOK(F("AT+GAPDEVNAME=Noisy Left Wheel")) ) {
    error(F("Could not set device name."));
  }

  // Add the IMU Service definition
  success = ble.sendCommandWithIntReply( F("AT+GATTADDSERVICE=UUID128=00-11-00-11-44-55-66-77-88-99-AA-BB-CC-DD-EE-FF"), &imuServiceId);
  if (! success) {
    error(F("Could not add Orientation service."));
  }

  // Add the Orientation characteristic
  success = ble.sendCommandWithIntReply( F("AT+GATTADDCHAR=UUID128=02-11-88-33-44-55-66-77-88-99-AA-BB-CC-DD-EE-FF,PROPERTIES=0x10,MIN_LEN=1,MAX_LEN=17,VALUE=\"\""), &orientationCharId);
  if (! success) {
    error(F("Could not add Orientation characteristic."));
  }

  // Add the Orientation Service to the advertising data
  // (needed for Nordic apps to detect the service)
  ble.sendCommandCheckOK( F("AT+GAPSETADVDATA=02-01-06-05-02-0d-18-0a-18") );

  // Reset the device for the new service setting changes to take effect
  ble.reset();
}

void orientation() {
  // Get Quaternion data (no 'Gimbal Lock' like with Euler angles)
  imu::Quaternion quat = bno.getQuat();
  float quatX = quat.x();
  float quatY = quat.y();
  float quatZ = quat.z();
  
  // Command is sent when \n (\r) or println is called
  // AT+GATTCHAR=CharacteristicID,value
  ble.print( F("AT+GATTCHAR=") );
  ble.print( orientationCharId );
  ble.print( F(",") );
  ble.print(String(quatX));
  ble.print( F(",") );
  ble.print(String(quatY));
  ble.print( F(",") );
  ble.println(String(quatZ));
}

bool compute_rotations(float axis, Rotations * rotations) {
  static float initial_axis_value = axis;
  // variable to store initial axis value in compute rotations - declared static so that it stores
  // this value in between function calls, but no other functions can change its value
  //Variables declared as static will only be created and initialized the first time a function is called

  float offset_rot = (axis-previous_axis_value) / 360; // offset since previous measurement, in rotations

  if(previous_axis_value == 666)  // so we do not account for anything in the setup phase
    offset_rot = 0;

  if(offset_rot >= 0)
    (rotations->forward_rotations) += offset_rot;
  else
    (rotations->reverse_rotations) += offset_rot;

  // place previous axis value
  previous_axis_value = axis;

  return(true); // returns true by default, do not remove, as it helps with the initial setup.
}

void rotation() {

}

void loop(void) {

  orientation();
  rotation();

  // Check if command executed OK
  if ( !ble.waitForOK() ) {
    error(F("Failed to get response!"));
  }

  // Delay before next measurement update
  delay(BNO055_SAMPLERATE_DELAY_MS);
}

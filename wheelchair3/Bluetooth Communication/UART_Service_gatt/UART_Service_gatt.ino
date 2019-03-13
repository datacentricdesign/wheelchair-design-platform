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

// BNO settings to delete
#define BNO055_SAMPLERATE_DELAY_MS (200)
// Creating our sensor object to handle the sensor, with initialization 12345
Adafruit_BNO055 bno = Adafruit_BNO055(55);



bool not_first_loop = false; // Boolean variable to stop logging of first loop
float previous_axis_value = 666;  // Initial value so we don't account for it

// GATT service information
int32_t UARTServiceId;
int32_t TxCharId;
int32_t RxCharId;

// A small helper
void error(const __FlashStringHelper*err) {
  if (Serial.available()) {
    Serial.println(err);
  }
  // In any case, turn on the LED to signal the error
  analogWrite(LED_PIN, HIGH);
  while (1);
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

  // Initialise the module (the Bluefruit feather)
  if ( !ble.begin(VERBOSE_MODE) ) {
    error(F("Couldn't find Bluefruit, make sure it's in CoMmanD mode & check wiring."));
  }

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


  // Add the IMU Service definition ("AT+GATTADDSERVICE")
  //****************************************************
  //ID of this service has 128-bit (UUID128=00-11-00-11-44-55-66-77-88-99-AA-BB-CC-DD-EE-FF) as all custom services. The value of this UUID, is random, as it has been created by me.
  //Officially adopted BLE Services have 16-bit IDs. For example, Heart Rate Service has addopted a 16-bit UUID of 0x180, (UUID=0x180D)
  success = ble.sendCommandWithIntReply( F("AT+GATTADDSERVICE=UUID=6E400001-B5A3-F393­E0A9­E50E24DCCA9E"), &UARTServiceId);
  if (! success) {
    error(F("Could not add Orientation service."));
  }


  // Add the TX characteristic
  //Used to send data back to the periferial node
  //****************************************************
  //En python la voy a referenciar con la variable GATT_CHARACTERISTIC_TX ="0x0002"
  success = ble.sendCommandWithIntReply( F("AT+GATTADDCHAR=UUID=0x0002,PROPERTIES=0x10,MIN_LEN=1,MAX_LEN=17,VALUE=\"\""), &TxCharId);
  if (! success) {
    error(F("Could not add TX characteristic."));
  }

  // Add the RX characteristic
  //Used to send data out to the connected Central device
  //****************************************************
  //En python la voy a referenciar con la variable GATT_CHARACTERISTIC_TX ="0x0003"
  success = ble.sendCommandWithIntReply( F("AT+GATTADDCHAR=UUID=0x0003,PROPERTIES=0x10,MIN_LEN=1,MAX_LEN=17,VALUE=\"\""), &RxCharId);
  if (! success) {
    error(F("Could not add RX characteristic."));
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
  ble.print( RxCharId );
  ble.print( F(",") );
  ble.print(String(quatX));
  ble.print( F(",") );
  ble.print(String(quatY));
  ble.print( F(",") );
  ble.println(String(quatZ));
}


void loop(void) {

  orientation();

  // Check if command executed OK
  if ( !ble.waitForOK() ) {
    error(F("Failed to get response!"));
  }

  // Delay before next measurement update
  delay(BNO055_SAMPLERATE_DELAY_MS);
}

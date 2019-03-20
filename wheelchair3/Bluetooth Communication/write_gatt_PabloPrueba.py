# This example shows how to establish a connection
# with the DCD HUB using the credentials of a thing,
# and start reading the serial port to transfer the data.

# This is a typical case for a Python transferring data
# from an Arduino-like device.

# Import required library
from dotenv import load_dotenv
import os
import serial
import pygatt
import time

from dcd.entities.thing import Thing
from dcd.entities.property_type import PropertyType

import requests

# The thing ID and access token
load_dotenv()

"""
For connecting to the Thing HUB
"""
THING_ID = os.environ['THING_ID']
THING_TOKEN = os.environ['THING_TOKEN']
# Instantiate a thing with its credential
my_thing = Thing(thing_id=THING_ID, token=THING_TOKEN)
# We can read the details of our thing,
# i.e. retrieving its information from the hub
my_thing.read()


"""
For connecting to the Bluetooth feather
"""
BLUETOOTH_DEVICE_MAC = os.environ['BLUETOOTH_DEVICE_MAC']

# UUID of the GATT characteristic to subscribe
GATT_CHARACTERISTIC_LED = "00002345-0000-1000-8000-00805f9b34fb"
# Many devices, e.g. Fitbit, use random addressing, this is required to connect.
ADDRESS_TYPE = pygatt.BLEAddressType.random
# Start a BLE adapter
bleAdapter = pygatt.GATTToolBackend()
bleAdapter.start()
# User the BLE adapter to connect to our device
my_device = bleAdapter.connect(BLUETOOTH_DEVICE_MAC, address_type=ADDRESS_TYPE)



def dcd_hub_status():
    """
    Return the DCD Hub status:
    - 0: Connection successful
    - 1: Could not reach the hub (Connection or Hub issue)
    """

    uri = "https://dwd.tudelft.nl/api/health"
    try:
        json_result = requests.get(uri, timeout=1).json()
        if json_result["status"] is 0:
            # We received a response with status = 0, everything is fine
            return 0
        # In any other case, there is a issue
        return 1
    except Exception as e:
        # Show some information about the error
        (str(e))
        # Return 1, the connection wasn't successful
        return 1


def discover_characteristic(device):
    for uuid in device.discover_characteristics().keys():
        try:
            print("Read UUID" + str(uuid) + "   " + str(device.char_read(uuid)))
        except:
            print("Something wrong with " + str(uuid))


def find_or_create(property_name, property_type):
    """Search a property by name, create it if not found, then return it."""
    if my_thing.find_property_by_name(property_name) is None:
        my_thing.create_property(name=property_name,
                                 property_type=property_type)
    return my_thing.find_property_by_name(property_name)


"""
Serial Communication (Raspberry-Mega)
"""
# Start reading the serial port
ser = serial.Serial(
    port = os.environ['SERIAL'],
    baudrate = 115200,
    timeout = 2)

# Read the next line from the serial port
# and update the property values
def serial_to_property_values():
    # Read one line
    line_bytes = ser.readline()
    print(line_bytes)
    # If the line is not empty
    if len(line_bytes) > 0:

        try:
            # Convert the bytes into string
            line = line_bytes.decode('utf-8')

            # Split the string using commas as separator, we get a list of strings
            values = line.split(',')
            print(values)
            lenght = ((len(values)+1)/2)
            print(lenght)

            counter = 0

            # Establishes the array property with
            for x in range(0, int(lenght)):
                propertyLine = values.pop(x)
                property = propertyLine.split('=')
                prop_name = property.pop(0)
                prop_value = [float(x) for x in property]

                print(prop_name, ' = ', prop_value)
                find_or_create(prop_name,
                               PropertyType.ONE_DIMENSION).update_values(
                               prop_value)

                if prop_name == "Button":
                    if int(prop_value) == 1:
                        my_device.char_write(GATT_CHARACTERISTIC_LED, bytearray([0x1, 0x00, 0x00]))
                    if int(prop_value) == 0:
                        my_device.char_write(GATT_CHARACTERISTIC_LED, bytearray([0x00, 0x00, 0x00]))

                # time.sleep(2)
        except:
            ("cant parse ")


# I read the characteristics defined by the feather
discover_characteristic(my_device)


# while true funciona como la void loop() en arduino,ejecutandose continuamente
while True:
    serial_to_property_values()

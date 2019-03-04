#!/usr/bin/env python3

# Import required library
import pygatt
import signal
from dotenv import load_dotenv
import os

from dcd.entities.thing import Thing
from dcd.entities.property_type import PropertyType

# The thing ID and access token
load_dotenv()
THING_ID = os.environ['THING_ID']
THING_TOKEN = os.environ['THING_TOKEN']
BLUETOOTH_DEVICE_MAC = os.environ['BLUETOOTH_DEVICE_MAC']

# UUID of the GATT characteristic to subscribe
GATT_CHARACTERISTIC = "2A37"

# Many devices, e.g. Fitbit, use random addressing, this is required to connect.
ADDRESS_TYPE = pygatt.BLEAddressType.random

def handle_data(handle, value_bytes):
    """
    handle -- integer, characteristic read handle the data was received on
    value_bytes -- bytearray, the data returned in the notification
    """
    print("Received data: %s" % str(value_bytes))
    values = [float(x) for x in value_bytes.decode('utf-8').split(",")]
    my_property.update_values(values)

# List characteristics of a device
def discover_characteristic(device):
    for uuid in device.discover_characteristics().keys():
        try:
            print("Read UUID" + str(uuid) + "   " + str(device.char_read(uuid)))
        except:
            print("Something wrong with " + str(uuid))

def read_characteristic(characteristic_id):
    return my_device.char_read(characteristic_id)

# Instantiate a thing with its credential
my_thing = Thing(thing_id=THING_ID, token=THING_TOKEN)

# We can read the details of our thing,
# i.e. retrieving its information from the hub
my_thing.read()

# If you just registered your Thing on the DCD Hub,
# it has only an id, a name and a type.
print(my_thing.to_json())

# If we have no properties, let's create a random one
if my_thing.find_property_by_name("My heart rate measurement") is None:
    # By specifying a property type, the DCD Hub will
    # automatically generate the property dimensions
    # (in this case, 3 generic dimensions)
    my_property = my_thing.create_property(
        name="My left wheel orientation",
        property_type=PropertyType.THREE_DIMENSIONS)

    # Let's have a look at the property, it should
    # contains the name, a unique id and the dimensions
    print(my_property.to_json())

# Retrieve the property
my_property = my_thing.find_property_by_name("My left wheel orientation")

# Show the property
print(my_property.to_json())

# Start a BLE adapter
bleAdapter = pygatt.GATTToolBackend()
bleAdapter.start()

# User the BLE adapter to connect to our device
my_device = bleAdapter.connect(BLUETOOTH_DEVICE_MAC, address_type=ADDRESS_TYPE)

# Subscribe to the GATT service
my_device.subscribe(GATT_CHARACTERISTIC, callback=handle_data)

# Make sure we close our program properly
def keyboard_interrupt_handler(signal_num, frame):
    print("Exiting...".format(signal_num))
    my_device.unsubscribe(GATT_CHARACTERISTIC)
    exit(0)

# Register our Keyboard handler to exit
signal.signal(signal.SIGINT, keyboard_interrupt_handler)
#!/usr/bin/env python
from __future__ import print_function

import binascii
import pygatt
import time

# Import required library
from dotenv import load_dotenv
import os
import serial

from dcd.entities.thing import Thing
from dcd.entities.property_type import PropertyType

# The thing ID and access token
load_dotenv()
THING_ID = os.environ['THING_ID']
THING_TOKEN = os.environ['THING_TOKEN']
BLUETOOTH_DEVICE_MAC = os.environ['BLUETOOTH_DEVICE_MAC']

GATT_CHARACTERISTIC = "02118833-4455-6677-8899-AABBCCDDEEFF"

# Many devices, e.g. Fitbit, use random addressing
# this is required to connect.
ADDRESS_TYPE = pygatt.BLEAddressType.random

def handle_data(handle, value_bytes):
    """
    handle -- integer, characteristic read handle the data was received on
    value_bytes -- bytearray, the data returned in the notification
    """
    print("Received data: %s" % str(value_bytes))
    values = [float(x) for x in value_bytes.decode('utf-8').split(",")]
    my_property.update_values(values)


# Instantiate a thing with its credential
my_thing = Thing(thing_id=THING_ID, token=THING_TOKEN)

# We can read the details of our thing,
# i.e. retrieving its information from the hub
my_thing.read()

# If you just registered your Thing on the DCD Hub,
# it has only an id, a name and a type.
print(my_thing.to_json())

# If we have no properties, let's create a random one
if my_thing.find_property_by_name("My left wheel orientation") is None:
    # By specifying a property type, the DCD Hub will
    # automatically generate the property dimensions
    # (in this case, 3 generic dimensions)
    my_property = my_thing.create_property(
        name="My left wheel orientation",
        property_type=PropertyType.THREE_DIMENSIONS)

    # Let's have a look at the property, it should
    # contains the name, a unique id and the dimensions
    print(my_property.to_json())


my_property = my_thing.find_property_by_name("My left wheel orientation")

print(my_property.to_json())


adapter = pygatt.GATTToolBackend()
adapter.start()
device = adapter.connect(BLUETOOTH_DEVICE_MAC, address_type=ADDRESS_TYPE)
device.subscribe(GATT_CHARACTERISTIC, callback=handle_data)

count = 0
while count < 10:
    time.sleep(1)
    count = count + 1

device.unsubscribe("02118833-4455-6677-8899-AABBCCDDEEFF")
#  print(str(device.char_read("6e400003-b5a3-f393-e0a9-e50e24dcca9e")))
#  for uuid in device.discover_characteristics().keys():
#    try:
#      print("Read UUID" + str(uuid) + "   "  + str(device.char_read(uuid)))
#    except:
#      print("Something wrong with " + str(uuid))

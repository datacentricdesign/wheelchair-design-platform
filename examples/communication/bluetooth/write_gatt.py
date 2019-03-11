#!/usr/bin/env python3

# Import required library
import pygatt
import signal
from dotenv import load_dotenv
import os
import time

from dcd.entities.thing import Thing
from dcd.entities.property_type import PropertyType

import requests

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
        print(str(e))
        # Return 1, the connection wasn't successful
        return 1

def discover_characteristic(device):
    for uuid in device.discover_characteristics().keys():
        try:
            print("Read UUID" + str(uuid) + "   " + str(device.char_read(uuid)))
        except:
            print("Something wrong with " + str(uuid))

# The thing ID and access token
load_dotenv()
THING_ID = os.environ['THING_ID']
THING_TOKEN = os.environ['THING_TOKEN']
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

while True:
    hub_status = dcd_hub_status()
    print(hub_status)
    if hub_status is 0:
        print("Internet available")
        my_device.char_write(GATT_CHARACTERISTIC_LED, bytearray([0xFF, 0x00, 0x00]))
    else:
        print("Internet not available")
        my_device.char_write(GATT_CHARACTERISTIC_LED, bytearray([0x00, 0x00, 0x00]))
    time.sleep(2)
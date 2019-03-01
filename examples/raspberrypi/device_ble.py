
#!/usr/bin/env python
from __future__ import print_function

import binascii
import pygatt
import time

YOUR_DEVICE_ADDRESS = "fb:48:5b:84:36:4a"
# Many devices, e.g. Fitbit, use random addressing - this is required to
# connect.
ADDRESS_TYPE = pygatt.BLEAddressType.random

def handle_data(handle, value):
    """
    handle -- integer, characteristic read handle the data was received on
    value -- bytearray, the data returned in the notification
    """
    print("Received data: %s" % hexlify(value))

adapter = pygatt.GATTToolBackend()
adapter.start()
device = adapter.connect(YOUR_DEVICE_ADDRESS, address_type=ADDRESS_TYPE)

device.subscribe("6e400003-b5a3-f393-e0a9-e50e24dcca9e", callback=handle_data)


while True:
    print(str(device.char_read("6e400003-b5a3-f393-e0a9-e50e24dcca9e")))
#  for uuid in device.discover_characteristics().keys():
#    try:
#      print("Read UUID" + str(uuid) + "   "  + str(device.char_read(uuid)))
#    except:
#      print("Something wrong with " + str(uuid))
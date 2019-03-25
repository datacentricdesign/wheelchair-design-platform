# This example shows how to establish a connection
# with the DCD HUB using the credentials of a thing,
# and start reading the serial port to transfer the data.

# This is a typical case for a Python transferring data
# from an Arduino-like device.

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

# Instantiate a thing with its credential
my_thing = Thing(thing_id=THING_ID, token=THING_TOKEN)

# We can read the details of our thing,
# i.e. retrieving its information from the hub
my_thing.read()

# Start reading the serial port
ser = serial.Serial(
    port = os.environ['SERIAL'],
    baudrate = 9600,
    timeout = 2)

# Read the next line from the serial port
# and update the property values
def serial_to_property_values():
    # Read one line
    line_bytes = ser.readline()
    # If the line is not empty
    if len(line_bytes) > 0:
        # Convert the bytes into string
        line = line_bytes.decode('utf-8')
        # Split the string using commas as separator, we get a list of strings
        values = line.split(',')
        try:
            # Use the first element of the list as property id
            property_id = values.pop(0)
            # Get the property from the thing
            prop = my_thing.properties[property_id]
            # If we find the property, we update the values (rest of the list)
            if prop is not None:
                prop.update_values([float(x) for x in values])
            # Otherwise, we show a warning
            else:
                print('Warning: unknown property ' + property_id)
        except:
            print('Could not parse: ' + line)

while True:
    serial_to_property_values()

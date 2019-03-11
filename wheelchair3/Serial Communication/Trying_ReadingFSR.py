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


def find_or_create(property_name, property_type):
    """Search a property by name, create it if not found, then return it."""
    if my_thing.find_property_by_name(property_name) is None:
        my_thing.create_property(name=property_name,
                                 property_type=property_type)
    return my_thing.find_property_by_name(property_name)


# Read the next line from the serial port
# and update the property values
def serial_to_property_values():
    # Read one line
    line_bytes = ser.readline()
    print(line_bytes)
    # If the line is not empty
    if len(line_bytes) > 0:
        # Convert the bytes into string
        line = line_bytes.decode('utf-8')

        try:
            # Split the string using commas as separator, we get a list of strings
            values = line.split(',')
            # print(values)
            lenght = len(values)/2

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
        except:
            print("cant parse ")
    # Finally, we call this method again
    serial_to_property_values()


serial_to_property_values()

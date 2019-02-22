# This example shows how to establish a connection
# with the DCD HUB using the credentials of a thing,
# and start reading the serial port to transfer the data.

# This is a typical case for a Python transferring data
# from an Arduino-like device.

# Import required library
from dotenv import load_dotenv
import os
import serial
import time

from dcd.entities.thing import Thing

# The thing ID and access token
load_dotenv()
THING_ID = os.environ['THING_ID']
THING_TOKEN = os.environ['THING_TOKEN']

DESC = ["Not Sitting", "Proper Sitting", "Leaning Forward", "Leaning Backward", "Leaning Left", "Leaning Right"]
MAX_SAMPLES = 100
DELAY_BETWEEN_POSTURE = 5000

def collect(posture_index):
    print("Get ready to collect the posture: " + DESC[posture_index]
          + " in " + str(DELAY_BETWEEN_POSTURE) + " seconds!")
    time.sleep(DELAY_BETWEEN_POSTURE)
    ser = open_serial()
    serial_to_property_values(posture_index, 0, ser)

def open_serial():
    # Start reading the serial port
    return serial.Serial(
    port = os.environ['SERIAL'],
    baudrate = 9600,
    timeout = 2)

# Read the next line from the serial port
# and update the property values
def serial_to_property_values(posture_index, sample_index, ser):
    # Read one line
    line_bytes = ser.readline()
    # If the line is not empty
    if len(line_bytes) > 0:
        # Convert the bytes into string
        line = line_bytes.decode('utf-8')
        # Split the string using commas as separator, we get a list of strings
        values = [float(x) for x in line.split(',')]

        milliseconds = int(round(time.time() * 1000))
        prop_sitting.update_values([milliseconds, posture_index])
        prop_fsr.update_values([milliseconds] + values)

    if sample_index < MAX_SAMPLES:
        serial_to_property_values(posture_index, sample_index+1, ser)
    else:
        ser.close()
        collect(posture_index+1)

# Instantiate a thing with its credential
my_thing = Thing(thing_id=THING_ID, token=THING_TOKEN)

# We can read the details of our thing,
# i.e. retrieving its information from the hub
my_thing.read()

prop_sitting = my_thing.find_property_by_name("Sitting Posture")
prop_fsr = my_thing.find_property_by_name("FSR")

collect(0)

# This example shows a basic way of collecting data with label. It prompts
# the user with the class (e.g 'Proper Sitting', 'Leaning Forward') to perform
# and it associates the corresponding label to the recorded data.

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

# Sitting classes
CLASSES = ["Not Sitting", "Proper Sitting", "Leaning Forward",
           "Leaning Backward", "Leaning Left", "Leaning Right"]

LABEL_PROP_NAME = "Sitting"
DATA_PROP_NAME = "fsr"

# How many samples do we want for each class
MAX_SAMPLES = 2000
# How much time (in seconds) to leave between the collection of each class
DELAY_BETWEEN_POSTURE = 15

# Collect data for a given posture
# posture_index: index of the class in the array CLASSES
def collect(class_index):
    # if we covered all classes, stop the program
    if class_index >= len(CLASSES):
        print("Data collection done.")
        exit()

    # Prompt the user to get ready and wait
    print("Get ready to collect the posture: " + CLASSES[class_index]
          + " in " + str(DELAY_BETWEEN_POSTURE) + " seconds!")
    time.sleep(DELAY_BETWEEN_POSTURE)

    # Open the serial connection
    print("Collecting data for posture " + CLASSES[class_index])
    ser = open_serial()

    # Start reading serial port with the posture index, start at sample 0.
    sample = 0
    while sample < MAX_SAMPLES:
        if serial_to_property_values(class_index, ser):
            sample += 1
            print()
    ser.close()
    collect(class_index + 1)



# Open a serial connection
def open_serial():
    # Start reading the serial port
    return serial.Serial(
        port=os.environ['SERIAL'],
        baudrate=9600,
        timeout=2)

# Read the next line from the serial port
# and update the property values
def serial_to_property_values(class_index, ser):
    # Read one line
    line_bytes = ser.readline()
    # If the line is not empty
    if len(line_bytes) > 0:
        # Convert the bytes into string
        line = line_bytes.decode('utf-8')
        # Split the string using commas as separator, we get a list of strings
        str_values = line.split(',')
        # Remove the first id
        str_values.pop(0)
        # Transform the array of string values into float values (numbers)
        values = [float(x) for x in str_values]

        # get the current time in milliseconds
        current_ts_ms = int(round(time.time() * 1000))
        # Update values of data and label properties (send them to the DCD Hub)
        # With the same timestamp, so we can easily connect label and raw data later
        prop_label.update_values([class_index], current_ts_ms)
        prop_data.update_values(values, current_ts_ms)

        return True
    return False


# Instantiate a thing with its credential
my_thing = Thing(thing_id=THING_ID, token=THING_TOKEN)

# Read the details of our Thing from the DCD Hub to get property details
my_thing.read()

# Find label and data property by name
prop_label = my_thing.find_property_by_name(LABEL_PROP_NAME)
prop_data = my_thing.find_property_by_name(DATA_PROP_NAME)

# Start collecting data for the first class
collect(0)

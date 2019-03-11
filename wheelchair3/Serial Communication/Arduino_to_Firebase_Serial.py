import firebase_admin
from firebase_admin import credentials
from firebase_admin import db

from dotenv import load_dotenv
import os
import serial

load_dotenv()
#Tell python where to find the credentials of my FireBase; which is
#a json whose file directory is indicated in the .env
cred = credentials.Certificate(os.environ['CREDENTIALS'])
print(cred)
# Initialize the app with a service account, granting admin privileges
firebase_admin.initialize_app(cred, {
    'databaseURL': 'https://connecting-firebase-and-framer.firebaseio.com/'
})

# Start reading the serial port
ser = serial.Serial(
    port=os.environ['SERIAL'],
    baudrate=9600,
    timeout=2)

ref = db.reference('/')
sensorReadings = ref.child('Sensor readings')


def find_or_create(property_name, property_value):

    if sensorReadings.child(property_name) is None:
        sensorReadings.set({property_name: property_value})
    else:
        sensorReadings.child(property_name).set(property_value)
    # print('ref = ')
    # print(ref.get())


# Read the next line from the serial port
# and update the property values
def serial_to_property_values():
    # Read one line
    line_bytes = ser.readline()
    #print(line_bytes)
    # If the line is not empty
    if len(line_bytes) > 0:

        try:
            # Convert the bytes into string
            line = line_bytes.decode('utf-8')
            # Split the string using commas as separator, we get a list of strings
            values = line.split(',')
            # print(values)
            lenght = len(values)/2

            # Establishes the array property with
            for x in range(0, int(lenght)):
                propertyLine = values.pop(x)
                property = propertyLine.split('=')
                prop_name = property.pop(0)
                prop_value = [int(x) for x in property]

                if prop_value is not None:
                    print(prop_name, ' = ', prop_value)
                    find_or_create(prop_name, prop_value)

        except:
            print("cant parse")
    # Finally, we call this method again
    serial_to_property_values()


serial_to_property_values()

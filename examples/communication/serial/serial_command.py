#!/usr/bin/env python3

# In this example, we send a command to the Arduino via the serial port.

# Import required library
import os                       # To access environment variables
from dotenv import load_dotenv  # To load environment variables from .env file
import serial                   # To connect via the serial port
import time                     # To sleep for a few seconds

# The thing ID and access token
load_dotenv()

# Start reading the serial port
ser = serial.Serial(
    port = os.environ['SERIAL'],
    baudrate = 9600,
    timeout = 2)

while True:
    ser.write('1')
    time.sleep(5)
    ser.write('0')
    time.sleep(2)
# Import required library
import os                       # To access environment variables
from dotenv import load_dotenv  # To load environment variables from .env file
import serial                   # To connect via the serial port
import time                     # To sleep for a few seconds

# The thing ID and access token
load_dotenv()

# Start reading the serial port
ser = serial.Serial(
    port = "/dev/ttyAMA0",
    baudrate = 9600,
    write_timeout = 0)

while True:
    print("Starting to print")
    ser.write('1'.encode())
    print("printing 1")
    time.sleep(5)
    print("printing 0")
    ser.write('0'.encode())
    time.sleep(2)

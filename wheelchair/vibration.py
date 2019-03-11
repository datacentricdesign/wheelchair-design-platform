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
    write_timeout = 0)

while True:
    print ("im starting")
    temp = '1'
    ser.write(temp.encode())
    print("write1")
    time.sleep(5)
    temp = '0'
    print("write 0")
    ser.write(temp.encode())
    time.sleep(2)

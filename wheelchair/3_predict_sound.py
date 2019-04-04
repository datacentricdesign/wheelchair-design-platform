from dotenv import load_dotenv
import os
import pickle
import serial
import numpy as np
import pygame
import pyaudio
import wave
import time

load_dotenv()

# Where to read the model from
MODEL_FILE_NAME = "model.pickle"

# load classifier
with open("model.pickle", 'rb') as file:
    neigh = pickle.load(file)

classes = ["No One", "Jelle", "Rosan",
           "William"]

# Read data from serial port
ser = serial.Serial(
    port=os.environ['SERIAL'],
    baudrate=9600,
    timeout=2)


# file: absolute path to WAV file to play
# duration: number of seconds to play in seconds
def play_sound(file, duration):
    CHUNK = 1024

    # Load the WAV file
    wf = wave.open(file, 'rb')

    p = pyaudio.PyAudio()

    stream = p.open(format=p.get_format_from_width(wf.getsampwidth()),
                    channels=wf.getnchannels(),
                    rate=wf.getframerate(),
                    output=True)

    data = wf.readframes(CHUNK)

    start_time = time.time()
    play = True
    while data != '' and play:
        stream.write(data)
        data = wf.readframes(CHUNK)

        if time.time()-start_time>duration:
            play = False

    stream.stop_stream()
    stream.close()

    p.terminate()



def predict(values):
    result = neigh.predict(values)
    print(classes[result[0]])

    while result["No One"]:
        play_sound('todd.wav', 500)

# Real time prediction
def serial_to_property_values():
    line_bytes = ser.readline()
    # If the line is not empty
    if len(line_bytes) > 0:
        # Convert the bytes into string
        line = line_bytes.decode('utf-8')
        str_values = line.split(',')
        if len(str_values) > 1:
            str_values.pop(0)
            values = [float(x) for x in str_values]
            values = [values]
            np.array(values).reshape(1, -1)
            predict(values)

serial_to_property_values()

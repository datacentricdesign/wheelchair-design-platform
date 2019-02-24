from dotenv import load_dotenv
import os
import pickle
import serial
import numpy as np

import pyaudio
import wave
import time

load_dotenv()

# Where to read the model from
MODEL_FILE_NAME = "model.pickle"

MAX_WRONG_POSTURE = 5000
wrong_posture_since = -1
nudged = False

# load classifier
with open("model.pickle", 'rb') as file:
    neigh = pickle.load(file)

classes = ["Not Sitting", "Proper Sitting", "Leaning Forward",
           "Leaning Backward", "Leaning Left", "Leaning Right"]

# Read data from serial port
ser = serial.Serial(
    port=os.environ['SERIAL'],
    baudrate=9600,
    timeout=2)

def predict(values):
    result = neigh.predict(values)
    print(classes[result[0]])
    return result

def action(posture):
    global wrong_posture_since
    global nudged
    if posture != 0 and posture != 1:
        current_ts_ms = int(round(time.time() * 1000))
        print("posture " + str(posture))
        print(wrong_posture_since)
        print(current_ts_ms)
        if wrong_posture_since != -1 and nudged == False and current_ts_ms-wrong_posture_since > MAX_WRONG_POSTURE:
            play_sound('/Users/jbourgeois/Desktop/Bass-loops_www.orangefreesounds.com/01-Bass-Loop-110-bpm.wav', 1)
            nudged = True
        elif wrong_posture_since == -1:
            wrong_posture_since = current_ts_ms
    else:
        wrong_posture_since = -1
        nudged = False

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
            action(predict(values))
    serial_to_property_values()

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

serial_to_property_values()
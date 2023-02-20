import paho.mqtt.client as mqtt
import threading
import random
import time

# SPDX-FileCopyrightText: 2021 ladyada for Adafruit Industries
# SPDX-License-Identifier: MIT

import board
import busio

# import adafruit_ads1x15.ads1015 as ADS
import adafruit_ads1x15.ads1115 as ADS
from adafruit_ads1x15.analog_in import AnalogIn

# Create the I2C bus
i2c = busio.I2C(board.SCL, board.SDA)

# Create the ADS object
# ads = ADS.ADS1015(i2c)
ads = ADS.ADS1115(i2c)

# Create a single-ended channel on Pin 0
#   Max counts for ADS1015 = 2047
#                  ADS1115 = 32767
chan = AnalogIn(ads, ADS.P0)

rate = [0]*10
amp = 100
GAIN = 2/3  
curState = 0
stateChanged = 0
last_BPM = 0

TOPIC = "/dpd/wheelchair/pulse"


from dotenv import dotenv_values
config = dotenv_values(".env")

# The callback for when the client receives a CONNACK response from the server.
def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))

    # Subscribing in on_connect() means that if we lose the connection and
    # reconnect then subscriptions will be renewed.
    # client.subscribe("test")
    # client.publish("hello", payload="test wheelchair", qos=0, retain=False)

    thread_hr = threading.Thread(target=read_pulse)
    thread_hr.start()

# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):
    print("Message on topic" + msg.topic+": "+str(msg.payload))

def on_pulse_data(data):
    client.publish(TOPIC, payload=data, qos=0, retain=False)

def read_pulse():
    firstBeat = True
    secondBeat = False
    sampleCounter = 0
    lastBeatTime = 0
    lastTime = int(time.time()*1000)
    th = 525
    P = 512
    T = 512
    IBI = 600
    Pulse = False

    while True:
        ads.gain = GAIN
        Signal = chan.value
        # print("{:5} {:5.3f}".format(chan.value, chan.voltage), end="")

        # Signal = adc.read_adc(0, gain=GAIN)   
        curTime = int(time.time()*1000)
        # send_to_prcessing("S",Signal)
        sampleCounter += curTime - lastTime
        lastTime = curTime
        N = sampleCounter - lastBeatTime

        if Signal > th and  Signal > P:          
            P = Signal
     
        if Signal < th and N > (IBI/5.0)*3.0 :  
            if Signal < T :                      
              T = Signal                                                 
        
        if N > 250 :                              
            if  (Signal > th) and  (Pulse == False) and  (N > (IBI/5.0)*3.0)  :       
              Pulse = 1;                       
              IBI = sampleCounter - lastBeatTime
              lastBeatTime = sampleCounter       

              if secondBeat :                     
                secondBeat = 0;               
                for i in range(0,10):             
                  rate[i] = IBI                      

              if firstBeat :                        
                firstBeat = 0                  
                secondBeat = 1                  
                continue                              

              runningTotal = 0;               
              for i in range(0,9):            
                rate[i] = rate[i+1]       
                runningTotal += rate[i]      

              rate[9] = IBI;                  
              runningTotal += rate[9]        
              runningTotal /= 10;             
              BPM = 60000/runningTotal       
              print("BPM:" + str(BPM))
              BPM = round(BPM * 2) / 2
              global last_BPM
              if BPM != last_BPM:
                on_pulse_data(BPM)
                last_BPM = BPM

        if Signal < th and Pulse == 1 :                    
            amp = P - T                   
            th = amp/2 + T
            T = th
            P = th
            Pulse = 0 
            

        if N > 2500 :
            th = 512
            T = th                  
            P = th                                              
            lastBeatTime = sampleCounter
            firstBeat = 0                     
            secondBeat = 0                   
            print("no beats found")

        time.sleep(0.005)

client = mqtt.Client(config["CLIENT_ID"])
client.on_connect = on_connect
client.on_message = on_message
client.username_pw_set(config["USERNAME"], config["PASSWORD"])
client.connect(config["HOST"], 1883, 60)

# Blocking call that processes network traffic, dispatches callbacks and
# handles reconnecting.
# Other loop*() functions are available that give a threaded interface and a
# manual interface.
client.loop_forever()


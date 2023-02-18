import paho.mqtt.client as mqtt
import threading
import random
import time

from dotenv import dotenv_values
config = dotenv_values(".env")

# The callback for when the client receives a CONNACK response from the server.
def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))

    # Subscribing in on_connect() means that if we lose the connection and
    # reconnect then subscriptions will be renewed.
    # client.subscribe("test")
    # client.publish("hello", payload="test wheelchair", qos=0, retain=False)

    thread_hr = threading.Thread(target=collect_hr_data)
    thread_hr.start()

# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):
    print("Message on topic" + msg.topic+": "+str(msg.payload))

def collect_hr_data():
    while True:
        on_hr_data(random.randint(0,100))
        time.sleep(2)

def on_hr_data(data):
    client.publish("hr", payload=data, qos=0, retain=False)

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


# Get Started

If you haven't set up your Raspberry Pi yet, look at the steps here.

In this tutorial we implement a basic setting on the Raspberry Pi:

* to collect the data from an Arduino
* to store this data in a file (on the Raspberry Pi)
* to transmit this data to a server via MQTT
* to run a machine algorithm on the Raspberry Pi

* Install Python 3 and dependencies

```
sudo apt-get install python3
```

```
pip3 install paho-mqtt
pip3 install sklearn
pip3 install pyserial

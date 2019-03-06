#!/usr/bin/env python3

# Import required library
from dotenv import load_doatenv
import pexpect
import time
import sys
import os

from dcd.entities.thing import Thing
from dcd.entities.property_type import PropertyType

def hexStrToInt(hexstr):
    val = int(hexstr[0:2], 16) + (int(hexstr[3:5], 16) << 8)
    if ((val & 0x8000) == 0x8000): # treat signed 16bits
        val = -((val ^ 0xffff) + 1)
    return val

hrmMacAddress = "e9:aa:88:b4:b6:bc"

# Instantiate a thing with its credential
my_thing = Thing(thing_id=THING_ID, token=THING_TOKEN)

# We can read the details of our thing,
# i.e. retrieving its information from the hub
my_thing.read()

# If you just registered your Thing on the DCD Hub,
# it has only an id, a name and a type.
print(my_thing.to_json())

# If we have no properties, let's create a random one
if my_thing.find_property_by_name("My heart rate measurement") is None:
    # By specifying a property type, the DCD Hub will
    # automatically generate the property dimensions
    # (in this case, 3 generic dimensions)
    my_property = my_thing.create_property(
        name="My heart rate measurement",
        property_type=PropertyType.THREE_DIMENSIONS)

    # Let's have a look at the property, it should
    # contains the name, a unique id and the dimensions
    print(my_property.to_json())

# Retrieve the property
my_property = my_thing.find_property_by_name("My heart rate measurement")

# Show the property
print(my_property.to_json())

# Spawn a child process with gatttool to control your BLE device. 
child = pexpect.spawn("sudo gatttool -t random -b {0} -I".format(hrmMacAddress) )

#Connect to hrm
print("Searchinf for HRM")
print("Connecting...")

# The number of times you want to retry connecting before you give up
RETRY_CONNECTION = 1

while True:
    try:
        child.sendline("connect")
        child.expect("Connection successful", timeout=5)
    except pexpect.TIMEOUT:
        RETRY_CONNECTION = RETRY_CONNECTION - 1
        if (RETRY_CONNECTION > 0):
            print("timeout, trying again")
            continue
        else:
            print("timeout, giving up.")
            break
    else:
        print("Connected!")
        break

#enable notification. 0x000f is found experimentally. You do not need to know this bit
#unless you are curious. 0100 is to switch on notifications for the particular characteristic.
child.sendline("char-write-req 0x000f 0100")

#read hrm data
while True:
    try:
        child.expect("Notification handle = 0x000e value: ", timeout=5)
        child.expect("\r\n", timeout=5)
        print(child.before)
        intvalue = hexStrToInt(child.before)
        print(intavalue)
    except KeyboardInterrupt:
        print("Exiting...")
        # Unsubscribe from characteristic before exiting program
        child.sendline("char-write-req 0x000f 0000")
        exit(0)


    

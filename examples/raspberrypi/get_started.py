# This example shows how to establish a connection
# with the DCD HUB using the credentials of a thing.

# This is a typical case for a Python code running
# on a device to collect data.

# Import required library
from random import random
import time

from dotenv import load_dotenv
import os

from dcd.entities.thing import Thing
from dcd.entities.property_type import PropertyType

# The thing ID and access token
load_dotenv()
THING_ID = os.environ['THING_ID']
THING_TOKEN = os.environ['THING_TOKEN']

# Instantiate a thing with its credential
my_thing = Thing(thing_id=THING_ID, token=THING_TOKEN)

# We can read the details of our thing,
# i.e. retrieving its information from the hub
my_thing.read()

# If you just registered your Thing on the DCD Hub,
# it has only an id, a name and a type.
print(my_thing.to_json())

# If we have no properties, let's create a random one
if my_thing.find_property_by_name("My Random Property") is None:
    # By specifying a property type, the DCD Hub will
    # automatically generate the property dimensions
    # (in this case, 3 generic dimensions)
    my_property = my_thing.create_property(
        name="My Random Property",
        property_type=PropertyType.THREE_DIMENSIONS)

    # Let's have a look at the property, it should
    # contains the name, a unique id and the dimensions
    print(my_property.to_json())

# Whether you have just created a property or you retrieved it
# from the DCD Hub (with my_thing.read), you can look for it by name
# WARNING: if you name two property with the same name, the Hub will
# create them both, but this function will randomly return the first
# it finds.
my_property = my_thing.find_property_by_name("My Random Property")

print(my_property.to_json())


# Let's create a function that generate random values
def generate_dum_property_values(the_property):
    # Define a tuple with the current time, and 3 random values
    values = (random(), random(), random())
    # Update the values of the property
    the_property.update_values(values)
    # Have a 2-second break
    time.sleep(2)
    # Then call the same function again to generate new values
    generate_dum_property_values(the_property)


# Finally, we call our function to start generating dum values
generate_dum_property_values(my_property)

from dotenv import load_dotenv
import os

from dcd.entities.thing import Thing

# The thing ID and access token
load_dotenv()
THING_ID = os.environ['THING_ID']
THING_TOKEN = os.environ['THING_TOKEN']

# Instantiate a thing with its credential
my_thing = Thing(thing_id=THING_ID, token=THING_TOKEN)

my_thing.read()

# Start recording
#
# property_name = 'WebCam'
# port = '/dev/video'
# segment_size = '30' size of each video segment in seconds
print('Starting video recording...')
my_thing.start_video_recording()

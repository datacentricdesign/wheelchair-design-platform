#!/usr/bin/env python3
import asyncio
from threading import Thread

import requests
import time
import math

from dcd.entities.thing import Thing
from dcd.entities.property_type import PropertyType

from dotenv import load_dotenv
import os
import subprocess

load_dotenv()
THING_ID = os.environ['THING_ID']
THING_TOKEN = os.environ['THING_TOKEN']

property_video_name = "Wheelchair Webcam"
current_video_file = None
current_video_start_ts = None

my_thing = Thing(thing_id=THING_ID, token=THING_TOKEN)
my_thing.read()


def find_or_create(property_name, property_type):
    """Search a property by name, create it if not found, then return it."""
    if my_thing.find_property_by_name(property_name) is None:
        my_thing.create_property(name=property_name,
                                 property_type=property_type)
    return my_thing.find_property_by_name(property_name)


def upload_file(file_name, start_ts, duration):
    print('uploading video file ' + file_name)

    property = find_or_create(property_video_name, PropertyType.VIDEO)

    files = {'video': (
        file_name, open('./' + file_name, 'rb'), 'video/mp4', {'Expires': '0'})}

    url = 'https://dwd.tudelft.nl/api/things/' + THING_ID + '/properties/' \
          + property.property_id + '/values/' + str(start_ts) + ',' + str(
        duration) + '/file'

    response = requests.post(url=url,
                             data={'values': [start_ts, duration]},
                             files=files,
                             headers={
                                 "Authorization": "bearer " + THING_TOKEN})

    print(response.status_code)


class SubprocessProtocol(asyncio.SubprocessProtocol):
    def pipe_data_received(self, fd, data):
        # if avconv is opening a new file, there is one ready to send
        line = str(data)
        if "Opening" in line:
            new_time = math.floor(time.time() * 1000)
            new_file = line.split("'")[1]
            print(new_file)
            global current_video_file, current_video_start_ts
            if current_video_file is not None:
                thread = Thread(target=upload_file,
                                args=(current_video_file,
                                      current_video_start_ts,
                                      new_time - current_video_start_ts,))
                thread.start()
            current_video_start_ts = new_time
            current_video_file = new_file

    def connection_lost(self, exc):
        loop.stop()  # end loop.run_forever()


loop = asyncio.get_event_loop()

try:
    loop.run_until_complete(loop.subprocess_exec(SubprocessProtocol,
                                                 "avconv",
                                                 "-f", "video4linux2",
                                                 "-i", "/dev/video0",
                                                 "-map", "0",
                                                 "-f", "segment",
                                                 "-segment_time", "30",
                                                 "-segment_format", "mp4",
                                                 "capture-%03d.mp4"))

    loop.run_forever()
finally:
    print("close")
    loop.close()

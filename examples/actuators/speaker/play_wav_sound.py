# This example shows how to play a WAV file.
# Documentation: http://people.csail.mit.edu/hubert/pyaudio/

# Import required library
import pyaudio
import wave
import time

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

play_sound('/Users/jbourgeois/Desktop/Bass-loops_www.orangefreesounds.com/01-Bass-Loop-110-bpm.wav', 1)
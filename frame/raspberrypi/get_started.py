#####################################################
# This script does the following things:            #
#                                                   #
# 1. Read the serial port for incoming messages     #
# 2. Connect as a client to MQTT broker             #
# 3. Publish incoming messages on the serial port   #
#    to the MQTT broker.                            #
#                                                   #
# Author: Dhaval Shah                               #
# Date: 8 August 2018.                              #
#                                                   #
#####################################################

import paho.mqtt.client as mqtt #import the client1
import time
import serial
import logging
import pathlib
import re
import io
import pickle

#enter topic here
jwt = "eyJhbGciOiJSUzI1NiIsInR5cCI6IkpXVCJ9.eyJpYXQiOjE1NDIzNDY5MjQsImV4cCI6MTg1NzkyNjUyNCwiYXVkIjoiaHR0cDovL2xvY2FsaG9zdDo4MDgwIn0.LVLRUX7tb6LVcaI9d5vMjeN2Jwfu4Eu9apw15zVGUs9Oo8BO2aCByZii7J5DydKpW2m_3tNT81QoLFZIchuZBiiYl4JW1dRIy8krUIlZgjoT_gyGX3o_GUTmE45vcHGSqcx5s06RwYpAgqgtS-hmKxRnyia7MDt0Vme5Eyb1d22lg94EqlHImwE-aOb5Aojans2KR1rjrnkGj0u06AzGcLrnNgXxoyfCc5jRKCNI2_LFbFqTrhWzpeg0I6B2D38An1OJR2ljXZr1zREWc2AOaevRjY651_K3lon2HXpRIl2Rz840MRAMet6dyl_BtOdTOAHdiQKuofJpX74AEj2FnNsQUjgzxdv6DS_IQq_2E75PRExI-UsrF9nA2q6pSVS2D6MeCErgW9krsieHfppfTrKJ5j8-qVfoSZKwZGRe_SNNtM0xdFduZLkvSndL7SmqRXSZEz6cXehWDQXD5gazS8OnGx5LqWiU2P3UWNHF5nKwGEN8zm3alVHx3nMkdxD-RaJHEOHg97w3gdeGVbh5CVTf3JGmgCLJFPwUtGviRUHXAmP65G0fjpLebqPegz2PHfpMols8_gWc84V-Wiw26hlKbbbi5DtvUIGpp-eKypbr_sh6dBZSLrlewxCD3RItgUT_Ffac7skgcEqZfnNy8Jv3vCOhhCpTY-2WSNnsylo"
thing_id = "test-d262"

topicMain = "/things/" + thing_id
topicWheel = "/wheelchair/wheel"


topicsend = "/actuators/commandstest" #enter topic here
topicpredict = "Posture"

topics = [(topicMain, 2), (topicWheel, 2), (topicsend, 1), (topicpredict, 0)] # (topic, qos)


broker = "10.0.1.16" #enter the address of the server here.
#broker = "iot.eclipse.org" , if you wish to use a web based server
port = 1883
mqttUser = thing_id
# mqttUser = "iot-wheelchair"
mqttPass = jwt
# mqttPass = "wheelchair01"
read_buffer = []

ser = serial.Serial(
    port = '/dev/ttyACM0',
    baudrate = 2000000,
    timeout = 2)

# blue = serial.Serial(
#     port = '/dev/rfcomm0',
#     baudrate = 9600,
#     timeout = 2)

fname = 'wheelchair-log.dat'   # log file to save data in
fname_disconnect = 'wheelchair-log_disconnect.dat'
fmode = 'ab+'             # log file mode = append

# the source file
folder = pathlib.Path("/home/pi/wheelchair/Dhaval/RPi/")    # located in this folder
filename = "wheelchair-log.dat"         # file name
filepath = folder / filename  # path object, defining the file

target = "/WheelchairIOT/"              # the target folder
targetfile = target + filename   # the target path and file name


with open('/home/pi/Downloads/classifier_1.pickle', 'rb') as file:
    neigh = pickle.load(file)

dataStream = ' '
i = 0
testDataset = []

def posture_predict():

    global testDataset
    dataStream = ser.readline()
    strValues = dataStream.split(',')
    if len(strValues) == 17: #check if there are 17 values in a line, skip the line with less values
        array = []
        for item in strValues: #write each item of each line into an array
            array.append(float(item))
            if len(array) == 17:
                testDataset.append(array)
                testData = []
                if len(testDataset) % 45 == 0: #write every 45 arraies (0.5 sec) into a list
                    temp = []
                    n = 1
                    for i in range(len(testDataset)):
                        if n % 45 > 0:
                            for item in testDataset[i]:
                                temp.append(item)
                            i = i + 1
                            n = n + 1
                        else:
                            for item in testDataset[i]:
                                temp.append(item)
                            testData.append(temp)
                            testDataset = []
                            temp = []
                            n = 1
                            i = 0
                            result = neigh.predict(testData) #predict
                            testData = []
                            for item in result:
                                if item == 0:
                                    print('Sleep mode')
                                if item == 1:
                                    print('Proper sitting')
                                if item == 2:
                                    print('Improper posture: Leaning forward')
                                if item == 3:
                                    print('Improper posture: Leaning backward')
                                if item == 4:
                                    print('Left lifted')
                                if item == 5:
                                    print('Right lifted')
                                if item == 6:
                                    print('Body lifted')
                                if item == 7:
                                    print('Arm sideway')
                                if item == 8:
                                    print('Arm front')

# This function contains print statements for subscribe event. Logging
# may be ignored.
def subscribe_topics(client,topics,qos = 0):
    print("topic ", topics,"  ", qos)
    if type(topics) is not list: #topics should be list of tuples
        if type(topics) is not tuple: #topics isn't tuple?
            topic_list = [(topicMain, 2), (topicWheel, 2), (topicsend, 2), (topicpredict, 2)]
        else:
            topic_list = [topics]
    else:
        topic_list = topics
    try:
        r = client.subscribe(topic_list)
        if r[0] == 0:
            logging.info("subscribed to topic " + str(topic_list) + " return code" +str(r))
            client.topic_ack.append([topic_list,r[1],0]) #keep track of subscription
        else:
            logging.info("error on subscribing " + str(r))
            print("error on subscribing " + str(r))
            return -1
    except Exception as e:
        logging.info("error on subscribe" + str(e))
        return -1
    return r

#Callback function for subscribe.
def on_subscribe(client,userdata,mid,granted_qos):
    """removes mid valuse from subscribe list"""
    logging.debug("in on subscribe callback result " + str(mid))
    if len(client.topic_ack) == 0:
        #print("All subs acknowledged")
        return
    for index,t in enumerate(client.topic_ack):
        if t[1] == mid:
            client.topic_ack.pop(index)#remove it
            #print("removed from list")

# checks if subscription to topics was successful
def check_subs(client):
    wcount = 0
    while wcount < 10: #wait loop
        if len(client.topic_ack) == 0:
            return True
        time.sleep(1)
        wcount += 1
        if not client.running_loop:
            client.loop(.01)  #check for messages manually
    return False

# logging function
def on_log(client, userdata, level, buf):
    print(buf)

# callback function for when a topic receives messages. Unnecessary for
def on_message(client, userdata, message):

    #   print "message received " ,str(message.payload.decode("utf-8"))
    #    print "message topic=",message.topic
    #    print "message qos=",message.qos
    #    print "message retain flag=",message.retain

    client.messagereceived_flag = True

    if message.topic == "/actuators/commandstest":
        inputcommand = message.payload
        if len(inputcommand) > 0:
            if inputcommand == "Abort":
                client.disconnect()
            elif inputcommand == "Predict":
                client.modeswitch_flag = False
            elif inputcommand == "Default":
                client.modeswitch_flag = True
            # elif inputcommand == "Wheel":
            # if client.modeswitch_flag:
            # blue.write('<' + message.payload + '>')
            # else:
            #     print "Invalid command. Wheel has been turned off"

#result code 0 indicates successful connection
#result code 5 indicates authentication error -> change username & password
# callback function for connection to client
def on_connect(client, userdata, flags, rc):
    if rc == 0:
        client.connected_flag = True #set flag
        print("connected OK")
    else:
        print("Bad connection Returned code=",rc)

# callback function for disconnection with client. Not used in the default
# code.
def on_disconnect(client, userdata, rc):
    client.connected_flag = False
    print ("client disconnected ok")

# callback function on message publication. Not used currently
def on_publish(client, userdata, mid):
    time.sleep(1)
    print ("In on_pub callback mid= "  ,mid)

mqtt.Client.connected_flag = False #create flag in class
mqtt.Client.topic_ack = [] #create topic acknowledgement list in class
mqtt.Client.running_loop = False #create topic acknowledgement list in class
mqtt.Client.messagereceived_flag = False
mqtt.Client.modeswitch_flag = True


client = mqtt.Client("Wheelchair",False)       #create client object
client.on_subscribe = on_subscribe   #assign function to callback
client.on_disconnect = on_disconnect #assign function to callback
client.on_connect = on_connect #assign function to callback
client.on_message = on_message
# client.on_publish = on_publish
client.username_pw_set(username = mqttUser, password = mqttPass) #enter username and password

try:
    client.connect(broker, port, keepalive=30) #establish connection
    client.loop_start()
except:
    with ser as pt, open(fname_disconnect, fmode) as log:
        while (1):
            read_serial = ser.readline()
            log.write("0\t")
            log.write(read_serial)
            log.flush()

time.sleep(1)
time.sleep(6)
print ("Subscribing to several topics ",topics)
r = subscribe_topics(client,topics)    #subscribe several topic

if not check_subs(client):
    print ("missing subscribe acknowledgemets")

print ("subscribed return = ",r)
time.sleep(6)

with ser as pt, open(fname,fmode) as outf:

    while client.connected_flag:
        try:
            posture_predict()
        except:
            print("posture error")
        read_serial = ser.readline()
        if client.modeswitch_flag:
            # try:
            # blue.open()
            # blue.isOpen()
            # except IOError:
            # blue.close()
            # blue.open()
            # read_blue = blue.readline()
            if client.messagereceived_flag:
                client.messagereceived_flag = False
                outf.write("1\t")
                client.publish(topicMain, read_serial)
                outf.write(read_serial)
                ser.flush()
                # client.publish(topicWheel, read_blue)
                # outf.write(read_blue)
                # blue.flush()
                outf.flush()
            else:
                outf.write("0\t")
                client.publish(topicMain, read_serial)
                outf.write(read_serial)
                ser.flush()
                # client.publish(topicWheel, read_blue)
                # outf.write(read_blue)
                # blue.flush()
                outf.flush()
        else:
            if client.messagereceived_flag:
                client.messagereceived_flag = False
                outf.write("2\t")
                client.publish(topicMain, read_serial)
                outf.write(read_serial)
                ser.flush()
                outf.flush()
            else:
                outf.write("3\t")
                client.publish(topicWheel, read_serial)
                outf.write(read_serial)
                ser.flush()
                outf.flush()



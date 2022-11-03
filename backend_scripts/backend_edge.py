# edge compute or server

# script for calculate:
# - Downhole dynachart
# - Predict Condition
# - Peak and minimun load
# - FillPump
# - SPM
# - production today
# - production prediction
# - run time
# - Datetime

# send configuration data, load from data base
# - Time sleep
#  
import json
import paho.mqtt.client as mqtt
from datetime import datetime

broker = 'broker.hivemq.com'
port = 1883
topic = 'oilAIOT/data/#'
client_id = 'edge2094390'
username = 'gle'
password = 'glettxx'

data = {"well01":{}}

def Fillage(position, load):
    fillage = 60
    return fillage

def on_connect(client, userdata, flags, rc):
    print("Connected with result code " + str(rc))
    print("UserData= " + str(userdata))
    print("flags= " + str(flags))
    print("")
    client.subscribe(topic)

def on_message(client, userdata, message):
    data_in = str(message.payload.decode("utf-8"))
    topic_in = str(message.topic)

    local_dt = datetime.now()

    m_mqtt = json.loads(data_in)

    if "pos" in m_mqtt:
        data[m_mqtt["Well"]]["pos"] = m_mqtt["pos"]

    if "load" in m_mqtt:
        data[m_mqtt["Well"]]["load"] = m_mqtt["load"]
    
    #build_edge_payload()
    
    payload = "[{}]: {} - {}".format(local_dt, topic_in, data_in)
    print(topic_in, data_in)

    

    epi_lon = m_mqtt["longitud"]
    epi_lat = m_mqtt["latitud"]
    lon = -76.8757
    lat = -11.9525 
    radius = m_mqtt["impacto"]

    #if topic_in = "saspe/sirene/main" or topic_in = "saspe/sirene/test":
    #if topic_in == "saspe/sirene/main":
    #    activate_sirene(epi_lat, epi_lon, lat, lon, radius)
        #activate_sirene(epi_X, epi_Y, COORD_X, COORD_Y, radius)

local_dt = datetime.now()

client = mqtt.Client(client_id, userdata="gledb")
client.username_pw_set(username, password)
client.on_connect = on_connect
client.on_message = on_message

client.connect(broker, port, 60)
client.loop_forever()
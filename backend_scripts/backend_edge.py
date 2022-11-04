# edge compute or server

# script for calculate:
# - Downhole dynachart
# - Predict Condition
# - Peak and minimun load ---- ok
# - FillPump
# - SPM ---- ok
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
import time

import numpy as np

broker = 'broker.hivemq.com'
port = 1883
topic = 'jphOandG/#'
topic_pub = 'jphOandG/'
client_id = 'edge2094390'
username = 'gle'
password = 'glettxx'

data = {}

def config():
    pass

# ------------------ PROCCESS --------------

def downChart(pos, load):

    array_pos = np.fromstring(pos, dtype=float, sep=',')
    array_load = np.fromstring(load, dtype=float, sep=',')

    pos = str(len(array_pos)) + "," + pos
    load = str(len(array_load)) + "," + load


    fillage = {"pos":pos, "load":load}

    return fillage

def diagnosis(pos, load):
    Diagnosis = "FLUID STROKE"
    return Diagnosis

def fillPump(pos, load):
    fillage = 60
    return fillage

def productionToday(well,FillPump,SPM):
    production = 23
    return 24

def productionPredcition(pos, load):
    condition = 26
    return 26

def runTime(well):
    # check well in database for day
    Runtime = 5
    return Runtime

def sendData(well,payload):
    time.sleep(5)
    topic = topic_pub + well + "/data"
    client.publish(topic, payload)
    print("send dataaaaa")

def proccess(well,data_proccess):
    #print(well,raw_data)
    #well = well
    pos = data_proccess["pos"]
    load = data_proccess["load"]

    print("*********")
    print(pos,load)

    dt = datetime.now()
    local_dt = dt.strftime("%Y-%m-%d %H:%M")
    SPM = data_proccess["data"][2]

    FillPump = fillPump(pos, load)
    Diagnosis = diagnosis(pos, load)
    DownChart = downChart(pos, load)
    RunTime = runTime(well)
    ProductionToday = productionToday(well,FillPump,SPM)

    print("********************")
    
    dictPayload = {
        "well":well,
        "DT":local_dt,
        "fillPump":FillPump,
        "diagnosis":Diagnosis,
        "downChart":DownChart,
        "runTime":RunTime,
        "productionToday":ProductionToday
        }

    payload = json.dumps(dictPayload)

    print(payload)

    sendData(well,payload)

    return 0

def on_connect(client, userdata, flags, rc):
    print("Connected with result code " + str(rc))
    print("UserData= " + str(userdata))
    print("flags= " + str(flags))
    print("")
    client.subscribe(topic)

def on_message(client, userdata, message):
    data_in = str(message.payload.decode("utf-8"))
    topic_in = str(message.topic)

    if topic_in == "oilAIOT/config":
        config()
        return 0
    
    if data_in == "Device conected":
        return 0
    else:
        m_mqtt = json.loads(data_in)
        
        if "Data" in m_mqtt and m_mqtt["Data"] == "stopped":
            print(m_mqtt["Well"],m_mqtt["Data"])
            return 0
        else:
            #print(m_mqtt)
            if 'Data' in m_mqtt:
                #print("dataaa", m_mqtt["Data"])
                data[m_mqtt["Well"]] = {"data": m_mqtt["Data"]}
                #data[m_mqtt["Well"]].update({"data":m_mqtt["Data"]})#{"data":m_mqtt["Data"]}
                
            elif 'pos' in m_mqtt:
                #data[m_mqtt["Well"]] = {"pos":m_mqtt["pos"]}
                data[m_mqtt["Well"]].update({"pos":m_mqtt["pos"]})
                #data[m_mqtt["Well"]]["pos"] = m_mqtt["pos"] #{"pos":m_mqtt["pos"]}
                #print(m_mqtt["pos"])

            elif 'load' in m_mqtt:
                print("complete")
                data[m_mqtt["Well"]].update({"load":m_mqtt["load"]})
                #data[m_mqtt["Well"]]["load"] = m_mqtt["load"]
                #print(m_mqtt["load"])
            else:
                print("default")
            
            if "data" and "pos" and "load" in data[m_mqtt["Well"]]:
                print("proccessssss")
                proccess(m_mqtt["Well"],data[m_mqtt["Well"]])
                data[m_mqtt["Well"]] = {}

            print("mmmmmjmjmjm -------------------------- ",data)
            
            

    #print(data)
    
    

    #if topic_in = "saspe/sirene/main" or topic_in = "saspe/sirene/test":
    #if topic_in == "saspe/sirene/main":
    #    activate_sirene(epi_lat, epi_lon, lat, lon, radius)
        #activate_sirene(epi_X, epi_Y, COORD_X, COORD_Y, radius)



client = mqtt.Client(client_id, userdata="gledb")
client.username_pw_set(username, password)
client.on_connect = on_connect
client.on_message = on_message

client.connect(broker, port, 60)
client.loop_forever()
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

import cv2
import matplotlib.pyplot as plt
import tensorflow as tf
from tensorflow import keras

import numpy as np

broker = 'broker.hivemq.com'
port = 1883
topic = 'jphOandG/#'
topic_pub = 'jphOandG/'
client_id = 'edge20221111'
username = 'gle'
password = 'glettxx'

data = {}

dict_targets = {
    0:'Full pump',
    1:'Leak travel valve',
    2:'Leak standing valve',
    3:'Worn pump barrel',
    4:'Light fluid stroke',
    5:'Medium fluid stroke',
    6:'Severe fluid stroke',
    7:'Gas interference',
    8:'Shock of pump up',
    9:'Shock of pump down'
}

dict_recomendation = {
    0:'Good work area',
    1:'Schedule to workover',
    2:'Schedule to workover',
    3:'Schedule to workover',
    4:'Good work area',
    5:'Reduce strokes per minute',
    6:'Reduce strokes per minute',
    7:'Apply gle eliminator',
    8:'Unit re-spacing',
    9:'Unit re-spacing'
}

def config():
    pass

# ------------------ PROCCESS --------------

def preproccesing(pos,load):
    Linewidth = 15
    plt.clf()
    plt.plot(pos,load, linewidth = Linewidth, color='black')
    plt.axis('off')
    plt.subplots_adjust(left=0.01, right=1, top=1, bottom=0.01)

    file = 'image.png'

    plt.savefig(file)

    img = cv2.imread(file)
    img = cv2.resize(img, (50,50))
    img = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

    img = img.clip(max=1)
    img = 1-img
    return img

def downChart(pos, load):

    #array_pos = np.fromstring(pos, dtype=float, sep=',')
    #array_load = np.fromstring(load, dtype=float, sep=',')
    pos_s = ','.join(str(x) for x in pos)
    load_s = ','.join(str(x) for x in load)

    pos = str(len(pos)) + "," + pos_s
    load = str(len(load)) + "," + load_s


    DownChar = {"pos":pos,"load":load}
    #print(DownChar)
    return DownChar

def diagnosis(pos, load):
    diagnosis_model = keras.models.load_model('CNN_regresion.h5')
    X = preproccesing(pos,load)
    value = diagnosis_model.predict(X.reshape([1, 50,50]))

    threshold, upper, lower = 0.6, 1, 0
    pred = np.where(value>threshold, upper, lower)
    pred = pred.flatten()
    index = np.where(pred==1)
    Diagnosis = [ dict_targets[int(i)] for i in index[0]]
    raw_diagnosis = str(Diagnosis).replace("[","").replace("]","").replace("'","")
    raw_diagnosis

    print("DIAGNOSISI ----->", raw_diagnosis)

    return raw_diagnosis

def fillPump(pos, load):

    fill_model = keras.models.load_model('fill_model.h5')
    X = preproccesing(pos,load)
    fill = fill_model.predict(np.expand_dims(X, axis=0))
    print("FILL ---->",fill[0][0])
    return int(fill[0][0])

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
    pos_s = data_proccess["pos"]
    load_s = data_proccess["load"]

    pos = [float(x) for x in pos_s.split(",")]
    load = [float(x) for x in load_s.split(",")]

    pos = np.array(pos)
    load = np.array(load)


    #print("*********")
    #print(pos, type(pos))
    #print(load)


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
    print(dictPayload)

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
                return 0
            
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

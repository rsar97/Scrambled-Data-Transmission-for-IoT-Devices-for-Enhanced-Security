import random
import random
import serial
import numpy as np
import time
from paho.mqtt import client as mqtt_client
from paho.mqtt import client as mqtt_client1

broker = 'public.cloud.shiftr.io'
port = 1883
y=5
client_id = 'RSSSSSS REQUEST'
username = 'public'
password = 'public'
topic = "ece508/finalproject/ECE508Team7/request"
topic1 = "ece508/finalproject/ECE508Team7/TRNG"
topic2 = "ece508/finalproject/ECE508Team7/temperature"


print("ECE 508 Final Project ")
print("Title: Secured Key Generation for Security Solutions")
print("Team Members: Raghul Saravanan, Anurag Mastyaraj, Sai Ram Kumar Kosuri")
print("Team No:7")

print("Welcome to the File Transfer System!")


def connect_mqtt():
    def on_connect(client, userdata, flags, rc):
        if rc == 0:
            print("Connected to MQTT Broker!")
        else:
            print("Failed to connect, return code %d\n", rc)

    client = mqtt_client.Client(client_id)
    client.username_pw_set(username, password)
    client.on_connect = on_connect
    client.connect(broker, port)
    return client




# PUBLISH THE REQUEST DATA
def publish(client):
        global G
    
        msg_count = 0
        for x in range(1):
            time.sleep(1)
            msg=D
            result = client.publish(topic, msg)
            # result: [0, 1]
            status = result[0]
            if status == 0:
               print("")
            else:
               print(f"Failed to send message to topic {topic}")
            msg_count += 1



G=""
D=""
def subscribe(client: mqtt_client):
    for x in range(10):
        def on_message(client, userdata, msg):
            global G
            global D
            G=str(msg.payload.decode())
            #print(type(G))
            D=int(G,2)
            print("Deicmal Value of the TRNG:",D)
            position = 0
            new_character = '1'
            #G = G[:position] + new_character + G[position+1:]
            transaction_id=str(random.randint(1000,5000))
            D=str(D)+request_id
            print("Request:",D)
            return G;
            client.disconnect()
            
        
            
        client.subscribe(topic1)
        client.on_message = on_message
       
def connect_mqtt1() -> mqtt_client1:
    def on_connect(client, userdata, flags, rc):
        if rc == 0:
            print("Connected to MQTT Broker!")
        else:
            print("Failed to connect, return code %d\n", rc)

    client = mqtt_client1.Client(client_id)
    client.username_pw_set(username, password)
    client.on_connect = on_connect
    client.connect(broker, port)
    return client

T=""
M=""
def subscribeg(client: mqtt_client1):
    def on_message(client, userdata, msg):
        global T
        T=str(msg.payload.decode())
        M=str(msg.payload.decode())
        client.disconnect()

    client.subscribe(topic2)
    client.on_message = on_message



        

print("To request for a file enter R")
mode =input("Please enter your selection:")



def run():
    client = connect_mqtt()
    client.loop_start()
    subscribe(client)
    publish(client)
    client.disconnect()
    client.loop_stop()


def run1():
    client = connect_mqtt()
    subscribeg(client)
    client.loop_forever()




if  mode=='R':
    request_id=str(random.randint(1000,5000))
    print("Your Transaction is being Processed! Your Request ID is:",request_id)
    r=1;
    run()
    if G=='':
        print(" OTP Genration Failed ! Try Again . Apologies for the inconveience ")
    else:
        print("otp sent to mail")
        authen=input("Enter Your OTP:")
        #print("BIN:",G)
        #print("DEC",D)
        #print("data:",T)
        if authen==D:
            print("CORRECT OTP")
            run1()
            print("check:",T)
            asc=''.join(format(ord(i), '08b') for i in T)
            print("sensor:",str(asc))
        else:
            run1()
            print("WRONG")
            print("Tempertaure Sensor Data:",T)
            asc1=''.join(format(ord(i), '08b') for i in T)
            rt=""
            for t in asc1:
                if t=='0':
                    rt+='1'
                else:
                    rt+='0'
            sliced=G[2:10]
            print("SLICED TRNG OUTPUT:",sliced)
            
            p=list(rt)
            q=list(sliced)
            r=q.count('1')
            #SCRAMBLING DATA
            
            for i in range (r):
                k=q.index('1')
                
                for j in range (k,len(p),8):
    
                    if p[j]==1:
                        p[j]=0
                    else:
                        p[j]=1
                
                    
           
            fin=' ' .join(map(str,p))
            print(fin)
   
    
else:
    print("Wrong Selection.Please Try Again")
   
    
    

    


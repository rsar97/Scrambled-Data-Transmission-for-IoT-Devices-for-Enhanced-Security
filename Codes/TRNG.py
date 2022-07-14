# python3.6

import random
import serial
import numpy as np
import time
from paho.mqtt import client as mqtt_client


broker = 'public.cloud.shiftr.io'
port = 1883
y=5
topic = "ece508/finalproject/ECE508Team7/gyro_value"
topic1 = "ece508/finalproject/ECE508Team7/TRNG"
# generate client ID with pub prefix randomly
client_id = 'RSSSSSS TRNG'
username = 'public'
password = 'public'
def fredkin(f0,f1,f2):
    a=f0;
    b=(not f0 and f1) ^ (f0 and f2)
    c=(f0 or f2) ^ (f0 and not (f1))
    return (a,b,c)
    
def fy(q,w,e):
    
    r=q
    s=q ^w
    t=q^e
    return(r,s,t)
    
def nft(k,l,z):
    
    f=k ^l;
    g=int((not l and not( z)) ^ (k and not(z)))
    v=(not l and z) ^ (k or (not(z)))
    return(f,g,v)

def connect_mqtt() -> mqtt_client:
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


def subscribe(client: mqtt_client):
    def on_message(client, userdata, msg):
        G=int(msg.payload.decode())
        print("The gyroscope value:",G)
        d=format(G, '09b')
        #print(d)
        x=d.zfill(10)
        print("Gyroscope values in Binary Format(10 Bits):",x)
        #trng design
        
        def designa(x)  :
            
            (OB1,o1,OB2)=fredkin(int(x[0]),int(x[1]),int(x[2]))
            (o2,o3,OB3)=nft(int(x[3]),int(x[4]),int(x[5]))
            (OB4,OB5,o4)=fredkin(OB1,o2,o3)
            (I1,I2,I3)=nft(int(x[6]),int(x[7]),int(x[8]))
            (OB6,OB7,I4)=fredkin(o4,I1,I2);    no=int(not(int(x[9])));
            (OB8,OB9,OB10)=nft(I4,I3,no);o_pB=np.array([OB1,OB2,OB3,OB4,OB5,OB6,OB7,OB8,OB9,OB10])
            print("The Output of TRNG :",o_pB)
            return (OB1,OB2,OB3,OB4,OB5,OB6,OB7,OB8,OB9,OB10)
        (b1,b2,b3,b4,b5,b6,b7,b8,b9,b10)=designa(x)
        outa=np.array([b1,b2,b3,b4,b5,b6,b7,b8,b9,b10])
        #print(type(outa))
        otp=''
        for a in outa:
            otp+=str(a)
        
        client.publish(topic1, otp)
        # publish TRNG 
    
    client.subscribe(topic)
    client.on_message = on_message


def run():
    client = connect_mqtt()
    subscribe(client)
    client.loop_forever()


if __name__ == '__main__':
    run()
    G=msg.payload.decode()
    print(G)

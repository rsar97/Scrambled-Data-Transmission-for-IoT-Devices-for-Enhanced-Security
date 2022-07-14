// This example uses an Arduino Nano 33 IoT together with
// a WiFiNINA to connect to shiftr.io.
//
//Arduino Nano 33 IoT library in https://github.com/256dpi/arduino-mqtt
//For MQTTX software use Host name "public:public@public.cloud.shiftr.io"

#include <stdio.h>
#include "myiot33_library.h"
#include <WiFiNINA.h>
#include <MQTT.h>
#include<SPI.h>
#include<Wire.h>
#include<Adafruit_GFX.h>
#include<Adafruit_SSD1306.h>
#include<SimpleDHT.h>
#include <Arduino_LSM6DS3.h>

 int sensor_value;
int pinDHT22 = 2;
SimpleDHT22 dht22(pinDHT22); 


const char gNumber[15] = "ECE508Team7";  //Update with your G-Number
const char ssid[31] = "AssAssin";     //Update with your WiFi SSID
const char pass[31] = "ggwpnt#123";     //Update with your WiFi password

unsigned long currMillis, prevMillis;
char lcdBuffer[64];
String oledline[9];
const int oledLib = 1;

WiFiClient wifiClient;
MQTTClient mqttClient;
//const char mqttBroker[63] = "192.168.0.51"; //Update with your computer IP. Use ipconfig in CMD window
//const char mqttBroker[63] = "192.168.0.51"; //Update with your computer IP. Use ipconfig in CMD window
//const char mqttBroker[63] = "ece508-001.eastus.cloudapp.azure.com";
const char mqttBroker[63] = "public.cloud.shiftr.io";

int myRand;
char mqttClienName[31] = "client__Gxxx20872";
int intervalMQTT = 0;
long nmrMqttMesages = 0;
String mqttStringMessage;
String a="sfsf aefeqfqefqeQF QEQfefeFEfaEFAEFDAefaerfqaWFQEFEQ";
char topicPub1[61]  = "";
char topicPub[61]  = "";


float temperature = 0;
float humidity = 0;
int errDHT22 = SimpleDHTErrSuccess;
enum State_enum {far, cel, none};
int state = cel;
int operationMode = 0;
int cmdSerial = 0;
int data01Serial = 0;
int data02Serial = 0;
//char topicSubHeartbeat[61]  = "";

void setup() {
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (true); // halt program
  } 
  Serial.println("IMU initialized!");
  
  Serial.begin(9600);
  randomSeed(analogRead(A7));
  sprintf(topicPub, "%s%s%s%s", "ece508/","finalproject/", gNumber, "/gyro_value");
  sprintf(topicPub1, "%s%s%s%s", "ece508/","finalproject/", gNumber, "/temperature");

  //Generate Random MQTT ClientID
  myRand = random(0, 9999);
  sprintf(mqttClienName, "client_%04d_%s", myRand, gNumber);
  
  iot33StartOLED(oledLib);
  oledline[1] = String(gNumber) + " Shiftr.io";
  for (int jj=2; jj<=8; jj++){ oledline[jj]=""; }
  
  // attempting to connect to Wifi network:  
  oledline[2] = "Connecting to Wifi..."; displayTextOLED(oledline, oledLib);
  prevMillis = millis();
  int status = WL_IDLE_STATUS;
  while (status != WL_CONNECTED) {    
    status = WiFi.begin(ssid, pass); delay(250);
  }
  sprintf(lcdBuffer, "Connected in %d sec", (millis()-prevMillis)/1000);
  oledline[2] = lcdBuffer; displayTextOLED(oledline, oledLib); delay(500);

  mqttClient.begin(mqttBroker, wifiClient);
  mqttClient.onMessage(messageReceived);
  
  prevMillis = millis();
}

void loop() {

  mqttClient.loop();

  if (!mqttClient.connected()) {
    for (int jj=2; jj<=8; jj++){ oledline[jj]=""; }
    oledline[2] = String("Connecting to MQTT..."); displayTextOLED(oledline, oledLib);
    connectMqtt(mqttClienName);
  }

  currMillis = millis();
  // publish a message roughly every 3  second.
  if (currMillis - prevMillis > 1000) {
    prevMillis = currMillis;

    oledline[2] = String(mqttBroker, 21);
    oledline[3] = String(mqttClienName, 21);
    
    intervalMQTT++;
    if (intervalMQTT == 1) {   
    
      //readsand publishes the temperature and the humidity for every 3 seconds
      accel ();
      temp_humd();
      intervalMQTT = 0;
      nmrMqttMesages++;
      oledline[4] = String("Send message: ") + String(nmrMqttMesages);
      //getMacWifiShieldMacRouterSS(lcdBuffer);
      mqttStringMessage =String (sensor_value);
      mqttClient.publish(topicPub, mqttStringMessage);
      mqttStringMessage = String(temperature)+String(humidity);
      mqttClient.publish(topicPub1, mqttStringMessage);
    }
          
    convDDHHMMSS(currMillis/1000, lcdBuffer);
    oledline[8] = "Uptime: " + String(lcdBuffer);
    displayTextOLED(oledline, oledLib);
  }
}



void accel (){
  float gX, gY, gZ;
  if (
    IMU.accelerationAvailable() 
    && IMU.gyroscopeAvailable()
  ) {      
    //IMU.readAcceleration(aX, aY, aZ);
    IMU.readGyroscope(gX, gY, gZ);
    float gX,gY,gZ;
    IMU.readGyroscope(gX, gY, gZ);
    sensor_value=abs(gY);
    Serial.print(sensor_value);
    //sprintf(lcdBuffer, "Temp:     %.2f F", sensor_value); oledline[7] = lcdBuffer;
  

  
}

}






void temp_humd () {

errDHT22 = dht22.read2(&temperature, &humidity, NULL);
    if (errDHT22 != 0)
    {
      temperature = (-1 - 32)/1.8;
      humidity = -1;
    }
    
    temperature = 1.8*temperature + 32;
    sprintf(lcdBuffer, "Temp:     %.2f F", (float)temperature); oledline[5] = lcdBuffer;
    sprintf(lcdBuffer, "Humidity: %.2f RH%%", (float)humidity); oledline[6] = lcdBuffer;

  
}
void connectMqtt(char *mqttClienName) 
{
  Serial.println("connectMqtt: Checking WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print("."); delay(1000);
  }
  Serial.println("connectMqtt: WiFi Ok...");
 
  Serial.println("connectMqtt: Checking MQTT...");
  while (!mqttClient.connect(mqttClienName, "public", "public")) {
    Serial.print("."); delay(1000);
  }
  Serial.println("connectMqtt: MQTT Ok...");
 
//  sprintf(topicSubHeartbeat, "%s%s", gNumber, "/heartbeat");
//  mqttClient.subscribe(topicSubHeartbeat);
//  Serial.println("Subscribe: " + String(topicSub));
  //mqttClient.subscribe("ece508/hello");
}


void messageReceived(String &topic, String &payload) {
  Serial.println("incoming: " + topic + " - " + payload);
}
  // Note: Do not use the client in the callback to publish, subscribe or
  // unsubscribe as it may cause deadlocks when other things arrive while
  // sending and receiving acknowledgments. Instead, change a global variable,
  // or push to a queue and handle it in the loop after calling `client.loop()`.

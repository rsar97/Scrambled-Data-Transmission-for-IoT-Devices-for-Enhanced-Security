#include <WiFi.h>
#include <PubSubClient.h>
#include <ESP_Mail_Client.h>
#include <Arduino.h>
#define SMTP_HOST "smtp.gmail.com"
#define SMTP_PORT 465
#define AUTHOR_EMAIL "roadredpo@gmail.com"
#define AUTHOR_PASSWORD "robin#fml"
#define RECIPIENT_EMAIL "rahulsaravanan97@gmail.com" 
SMTPSession smtp;

const char* ssid = "AssAssin";
const char* password =  "ggwpnt#123";
const char* mqttServer = "public.cloud.shiftr.io";
const int mqttPort = 1883;
const char* mqttUser = "public";
const char* mqttPassword = "public";
int topicsArry[4]={};

String messageTemp;
WiFiClient espClient;
PubSubClient client(espClient);
 
void callback(char* topic, byte* payload, unsigned int length) {
 
    payload[length] = '\0';
    String value = String((char*) payload);
    Serial.println(value);
    ESP_Mail_Session session;
    session.server.host_name = SMTP_HOST;
    session.server.port = SMTP_PORT;
    session.login.email = AUTHOR_EMAIL;
    session.login.password = AUTHOR_PASSWORD;
    session.login.user_domain = "";
    SMTP_Message message;
    message.sender.name = "Team 7 File Transfer";
    message.sender.email = AUTHOR_EMAIL;
    message.subject = "Your OTP";
    message.addRecipient("Sara", RECIPIENT_EMAIL);
    String textMsg = "Here is your OTP"+value;
    message.text.content = textMsg.c_str();
    message.text.charSet = "us-ascii";
    message.text.transfer_encoding = Content_Transfer_Encoding::enc_7bit;
    smtp.connect(&session);
    MailClient.sendMail(&smtp,&message);
    
    
        
    }
 
void setup() {
 
  Serial.begin(115200);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
 
  client.setServer(mqttServer, mqttPort);
 client.setCallback(callback);
 
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
 
    if (client.connect("RSSSS_ESP", mqttUser, mqttPassword )) {
 
      Serial.println("connected");  
 
    } else {
 
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
 
    }
  }
 
  client.subscribe("ece508/finalproject/ECE508Team7/request");
 
  //Serial.print("dfdf");

 
}
 
void loop() {

  client.loop();
  //Serial.print("dfsdf");
}

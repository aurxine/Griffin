#include <Arduino.h>
#include "nodeMCUpins.h"
#include<ESP8266WiFi.h>
#include "setupDevice.h"
#include "setupDevice.cpp"
#include "MQTTBroker.h"
#include <PubSubClient.h>
//#include<EspMQTTClient.h>
#include<ArduinoJson.h>

StaticJsonDocument<512> Configuration_Data;


#define Max_number_of_sensors 6


setupDevice nodeMCU;

MQTTBroker local_Client;

String Sub_Topic = "Sub/" + nodeMCU.ID();
String Pub_Topic = "Pub/" + nodeMCU.ID();
String payload_from_global;
String payload_from_local;

WiFiClient espWifi;
PubSubClient global_Client(espWifi);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE	(50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

void callback(char* topic, byte* payload, unsigned int length) {
  // Serial.print("Message arrived [");
  // Serial.print(topic);
  // Serial.print("] ");
  payload_from_global = "";
  for (int i = 0; i < (int)length; i++) {
    payload_from_global += (char)payload[i];
    //Serial.print((char)payload[i]);
  }
  Serial.println();

}

void reconnect() {
  // Loop until we're reconnected
  while (!global_Client.connected()) 
  {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String global_ClientId = "ESP8266Client-";
    global_ClientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (global_Client.connect(global_ClientId.c_str())) 
    {
      Serial.println("connected");
      // Once connected, publish an announcement...
      global_Client.publish(Pub_Topic.c_str(), "hello world");
      // ... and resubscribe
      global_Client.subscribe(Sub_Topic.c_str());
    } 
    else 
    {
      Serial.print("failed, rc=");
      Serial.print(global_Client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


int sensors[Max_number_of_sensors] = {D1, D2, D3, D4, D5, D6}; // D3 and D4 already pulled up
bool sensors_activated[Max_number_of_sensors] = {0, 0, 0, 0, 0, 0};
int Alarm_pin = D7;

void SendMessage(String message, String number)
{
  unsigned int len = number.length() + 1;
  char Number[len];
  number.toCharArray(Number, len);

  Serial.println(Number);
  Serial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1000 milli seconds or 1 second
  char outString[19];
  sprintf_P(outString, PSTR("AT+CMGS=\"%s\"\r"), Number);
  Serial.println(outString);
  delay(1000);
  Serial.println(message);// The SMS text you want to send
  delay(100);
  Serial.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
}

void sendMessagesToAllContacts()
{
  String msg = "Intruder Alert!";
  for(int i = 0; i < nodeMCU.NumberOfContacts(); i++)
  {
    SendMessage(msg, nodeMCU.Contact(i));
    delay(50000);
  }
}


const char* id ;
const char* ssid;
const char* password;
int number_of_sensors;
int number_of_contacts;
//const char* msg;

void Configure()
{
  while (local_Client.Data.length() <= 0)
  {
    //Serial.println(local_Client.Data.length());
    yield(); //do nothing
  }

  delay(1000);
  Serial.println(local_Client.Data);
  DeserializationError error = deserializeJson(Configuration_Data, local_Client.Data);

// Test if parsing succeeds.
  if (error) 
  {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
    return;
  }

  id = Configuration_Data["Device_ID"];
  nodeMCU.getID((String)id); 

  ssid = Configuration_Data["SSID"];
  nodeMCU.get_STA_SSID((String)ssid);
  
  password = Configuration_Data["Password"];
  nodeMCU.get_STA_Password((String)password);

  number_of_sensors = Configuration_Data["Number_of_Sensors"];
  nodeMCU.getNumberOfSensors(number_of_sensors);

  number_of_contacts = Configuration_Data["Number_of_Contacts"];
  nodeMCU.getNumberOfContacts(number_of_contacts);

  const char* msg = Configuration_Data["Message"];
  nodeMCU.getMessage(msg);
  delay(2000);

  for(int i = 1; i <= number_of_contacts; i++)
  {
    const char* contact = Configuration_Data["Contact_" + (String)i];
    nodeMCU.getContact((String)contact);
    Serial.println(contact);
    delay(500);
  }

  Sub_Topic = "Sub/" + nodeMCU.ID();
  Serial.println(Sub_Topic);
  local_Client.publish(Pub_Topic, "configured");
  nodeMCU.reset(0);
  delay(5000);
  ESP.restart();
}

void ICACHE_RAM_ATTR Motion_Detection()
{
  for(int i = 0; i < nodeMCU.NumberOfSensors(); i++)
  {
    sensors_activated[i] = digitalRead(sensors[i]);
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(Alarm_pin, OUTPUT);

  delay(5000);

  // nodeMCU.get_AP_SSID("Griffin");
  // nodeMCU.get_AP_Password("12345678");

  //nodeMCU.reset(1);
  nodeMCU.showMessage();

  if(nodeMCU.resetState())
  {
    nodeMCU.getID("Griffin0");
    nodeMCU.showID();

    if(nodeMCU.connectToAP())
    {
      Serial.println("AP started");
      nodeMCU.showSoftAPIP();
    }
    Sub_Topic = "Sub/" + nodeMCU.ID();
    Pub_Topic = "Pub/" + nodeMCU.ID();

    local_Client.init();
    local_Client.subscribe(Sub_Topic);

    Configure();
  }

  nodeMCU.showAllData();

  if(nodeMCU.connect())
  {
    Serial.println("WiFi Connected");
    nodeMCU.showLocalIP();
  }
  else
  {
    Serial.println("Couldn't connect to WiFi");
  }


  //Setting up sensors based on number of sensors
  for(int i = 0; i < nodeMCU.NumberOfSensors(); i++)
  {
    if(i != 2 && i != 3)
    {
      pinMode(sensors[i], INPUT_PULLUP);
    }
    else
    {
      pinMode(sensors[i], INPUT);
    }
    
    attachInterrupt(digitalPinToInterrupt(sensors[i]), Motion_Detection, RISING);
  }

  Serial.println(Sub_Topic);
  Serial.println(Pub_Topic);
  local_Client.init();
  global_Client.setServer("broker.hivemq.com", 1883);
  global_Client.setCallback(callback);

  local_Client.subscribe(Sub_Topic);
  //Serial.println(global_Client.subscribe(Sub_Topic, onMessageReceived));
  
}

int counter = 0;

void loop() {
  // if(local_Client.Data.length() > 0)
  // {
  //   Serial.println(local_Client.Data);
  // }
  if(!global_Client.connected())
  {
    reconnect();
  }

  Serial.println(payload_from_global);
  global_Client.loop();
  delay(2000);

}
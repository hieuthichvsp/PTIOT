#include <Arduino.h>
#line 1 "c:\\Users\\lolhi\\Documents\\Vsc_arduino\\PTIOT\\Tuan2\\sketch.ino"
#include <WiFi.h>
#include "PubSubClient.h"
#include "DHT.h"
#include <DHT_U.h>
#include "ArduinoJson.h"

JsonDocument doc;

#define DHTPIN 21
DHT dht(DHTPIN, DHT22);

const char *MQTTServer = "broker.emqx.io";
const char *MQTT_Topic = "22004266/Data";

// Tạo ID ngẫu nhiên tại: https://www.guidgen.com/
const char *MQTT_ID = "23109811-5b67-4e56-aa4d-1b26fa14451c";
int Port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

#line 22 "c:\\Users\\lolhi\\Documents\\Vsc_arduino\\PTIOT\\Tuan2\\sketch.ino"
void WIFIConnect();
#line 39 "c:\\Users\\lolhi\\Documents\\Vsc_arduino\\PTIOT\\Tuan2\\sketch.ino"
void MQTT_Reconnect();
#line 80 "c:\\Users\\lolhi\\Documents\\Vsc_arduino\\PTIOT\\Tuan2\\sketch.ino"
void setup();
#line 89 "c:\\Users\\lolhi\\Documents\\Vsc_arduino\\PTIOT\\Tuan2\\sketch.ino"
void loop();
#line 22 "c:\\Users\\lolhi\\Documents\\Vsc_arduino\\PTIOT\\Tuan2\\sketch.ino"
void WIFIConnect()
{
    dht.begin();
    Serial.println("Connecting to SSID: Wokwi-GUEST");
    //   WiFi.begin("Wokwi-GUEST", "");
    WiFi.begin("Hieu", "hieudzvd");
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.print("WiFi connected");
    Serial.print(", IP address: ");
    Serial.println(WiFi.localIP());
}

void MQTT_Reconnect()
{
    while (!client.connected())
    {
        if (client.connect(MQTT_ID))
        {
            Serial.print("MQTT Topic: ");
            Serial.print(MQTT_Topic);
            Serial.print(" connected");
            client.subscribe(MQTT_Topic);
            Serial.println("");
        }
        else
        {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            delay(5000);
        }
    }
}

// void callback(char* topic, byte* message, unsigned int length) {
//   Serial.print("Message arrived on topic: ");
//   Serial.println(topic);
//   Serial.print("Message: ");
//   String stMessage;

//   for (int i = 0; i < length; i++) {
//     Serial.print((char)message[i]);
//     stMessage += (char)message[i];
//   }
//   Serial.println();
//   if (stMessage == "on") {
//     digitalWrite(ledPin, HIGH);
//   }
//   else if (stMessage == "off") {
//     digitalWrite(ledPin, LOW);
//   }
// }

void setup()
{
    Serial.begin(115200);
    WIFIConnect();
    client.setServer(MQTTServer, Port);
    doc["temp"] = 0;
    doc["humi"] = 0;
}

void loop()
{
    delay(10);
    if (!client.connected())
    {
        MQTT_Reconnect();
    }
    int temp = dht.readTemperature();
    int humi = dht.readHumidity();
    doc['temp'] = temp;
    doc['humi'] = humi;
    Serial.print("Temperature: ");
    Serial.print(temp);
    Serial.print("°C, Humidity: ");
    Serial.print(humi);
    Serial.println("%");
    String jsonString = doc.as<String>();
    client.publish(MQTT_Topic, jsonString.c_str());
    client.loop();
}

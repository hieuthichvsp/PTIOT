#include <WiFi.h>
#include "PubSubClient.h"
#include <ArduinoJson.h>

const char *MQTTServer = "broker.emqx.io";
const char *MQTT_Topic = "22004266/LED";

// Tạo ID ngẫu nhiên tại: https://www.guidgen.com/
const char *MQTT_ID = "c37ad5a4-27b3-45a3-9598-c2cae418927d";
int Port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);
const int ledPin[] = {2, 4, 5};
int timeLed[] = {0, 0, 0};
int flag = 0;
void WIFIConnect()
{
    Serial.println("Connecting to SSID: Wokwi-GUEST");
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

void callback(char *topic, byte *message, unsigned int length)
{
    Serial.print("Message arrived on topic: ");
    Serial.println(topic);
    Serial.print("Message: ");
    String stMessage;
    DynamicJsonBuffer jsonBuffer;

    for (int i = 0; i < length; i++)
    {
        Serial.print((char)message[i]);
        stMessage += (char)message[i];
    }
    JsonObject &data = jsonBuffer.parseObject(stMessage);
    for (int i = 0; i < 3; i++)
    {
        String key = String(i + 1);
        timeLed[i] = data["led"][i][key.c_str()].as<int>() * 1000;
        Serial.println(timeLed[i]);
    }
    flag = 1;
    client.publish("22004266/RETURN", "Da nhan");
}

void setup()
{
    Serial.begin(115200);
    WIFIConnect();
    client.setServer(MQTTServer, Port);
    client.setCallback(callback);
    for (int i = 0; i < 3; i++)
    {
        pinMode(ledPin[i], OUTPUT);
    }
}

void loop()
{
    delay(10);
    if (!client.connected())
    {
        MQTT_Reconnect();
    }
    client.loop();
    int temp = 0;
    // Green Led
    while (temp + 10 < timeLed[0])
    {
        if (flag == 1)
        {
            client.loop();
            digitalWrite(ledPin[0], HIGH);
            delay(10);
            if (temp == timeLed[0])
                flag = 2;
        }
        else
        {
            digitalWrite(ledPin[0], LOW);
        }
    }
    // Yellow Led
    temp = 0;
    while (temp + 10 < timeLed[1])
    {
        if (flag == 2)
        {
            client.loop();
            digitalWrite(ledPin[1], HIGH);
            delay(10);
            if (temp == timeLed[1])
                flag = 3;
        }
        else
        {
            digitalWrite(ledPin[1], LOW);
        }
    }
    // Red Led
    temp = 0;
    while (temp + 10 < timeLed[2])
    {
        if (flag == 3)
        {
            client.loop();
            digitalWrite(ledPin[2], HIGH);
            delay(10);
            if (temp == timeLed[2])
                flag = 1;
        }
        else
        {
            digitalWrite(ledPin[2], LOW);
        }
    }
}
#include <Arduino.h>
#line 1 "c:\\Users\\lolhi\\Documents\\Vsc_arduino\\PTIOT\\Tuan1\\sketch.ino"
#include <WiFi.h>
#include "PubSubClient.h"
#include <LiquidCrystal_I2C.h>

const char *MQTTServer = "44.232.241.40";
const char *MQTT_Topic = "22004266/LED";

// Tạo ID ngẫu nhiên tại: https://www.guidgen.com/
const char *MQTT_ID = "1ca9c3d6-e2ef-4b87-a3b5-610ad4535929";
int Port = 1883;

LiquidCrystal_I2C lcd(0x27, 16, 2);

WiFiClient espClient;
PubSubClient client(espClient);

const int ledPin[] = {4, 5, 18, 19};

int flag = 0;

#line 21 "c:\\Users\\lolhi\\Documents\\Vsc_arduino\\PTIOT\\Tuan1\\sketch.ino"
void TuanTu();
#line 32 "c:\\Users\\lolhi\\Documents\\Vsc_arduino\\PTIOT\\Tuan1\\sketch.ino"
void NgauNhien();
#line 41 "c:\\Users\\lolhi\\Documents\\Vsc_arduino\\PTIOT\\Tuan1\\sketch.ino"
void CungLuc();
#line 55 "c:\\Users\\lolhi\\Documents\\Vsc_arduino\\PTIOT\\Tuan1\\sketch.ino"
void WIFIConnect();
#line 70 "c:\\Users\\lolhi\\Documents\\Vsc_arduino\\PTIOT\\Tuan1\\sketch.ino"
void MQTT_Reconnect();
#line 92 "c:\\Users\\lolhi\\Documents\\Vsc_arduino\\PTIOT\\Tuan1\\sketch.ino"
void callback(char *topic, byte *message, unsigned int length);
#line 139 "c:\\Users\\lolhi\\Documents\\Vsc_arduino\\PTIOT\\Tuan1\\sketch.ino"
void setup();
#line 154 "c:\\Users\\lolhi\\Documents\\Vsc_arduino\\PTIOT\\Tuan1\\sketch.ino"
void loop();
#line 21 "c:\\Users\\lolhi\\Documents\\Vsc_arduino\\PTIOT\\Tuan1\\sketch.ino"
void TuanTu()
{
    for (int i = 0; i < 4; i++)
    {
        digitalWrite(ledPin[i], HIGH);
        delay(500);
        digitalWrite(ledPin[i], LOW);
        delay(500);
    }
}

void NgauNhien()
{
    int i = random(0, 4);
    digitalWrite(ledPin[i], HIGH);
    delay(500);
    digitalWrite(ledPin[i], LOW);
    delay(500);
}

void CungLuc()
{
    for (int i = 0; i < 4; i++)
    {
        digitalWrite(ledPin[i], HIGH);
    }
    delay(500);
    for (int i = 0; i < 4; i++)
    {
        digitalWrite(ledPin[i], LOW);
    }
    delay(500);
}

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

    for (int i = 0; i < length; i++)
    {
        Serial.print((char)message[i]);
        stMessage += (char)message[i];
    }
    Serial.println();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Message: ");
    lcd.print(stMessage);
    if (stMessage == "tuantu")
    {
        flag = 1;
    }
    else if (stMessage == "ngaunhien")
    {
        flag = 2;
    }
    else if (stMessage == "cungluc")
    {
        flag = 3;
    }
    else if (stMessage == "bat")
    {
        flag = 0;
        for (int i = 0; i < 4; i++)
        {
            digitalWrite(ledPin[i], HIGH);
        }
    }
    else if (stMessage == "tat")
    {
        flag = 0;
        for (int i = 0; i < 4; i++)
        {
            digitalWrite(ledPin[i], LOW);
        }
    }
}

void setup()
{
    Serial.begin(115200);

    WIFIConnect();
    client.setServer(MQTTServer, Port);
    client.setCallback(callback);
    for (int i = 0; i < 4; i++)
    {
        pinMode(ledPin[i], OUTPUT);
    }
    lcd.backlight();
    lcd.init();
}

void loop()
{
    delay(10);
    if (!client.connected())
    {
        MQTT_Reconnect();
    }
    if (flag == 1)
    {
        TuanTu();
    }
    else if (flag == 2)
    {
        NgauNhien();
    }
    else if (flag == 3)
    {
        CungLuc();
    }
    client.loop();
}

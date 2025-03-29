#include <WiFi.h>
#include "PubSubClient.h"
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);
const char *MQTTServer = "broker.emqx.io";
const char *MQTT_Topic = "22004266/lcd";

// Tạo ID ngẫu nhiên tại: https://www.guidgen.com/
const char *MQTT_ID = "4c6b5930-4619-4d77-ab66-4f7534392807";
int Port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

void WIFIConnect()
{
    Serial.println("Connecting to SSID: Wokwi-GUEST");
    WiFi.begin("Wokwi-GUEST", "");
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
    int dem = 0;
    int dem1 = 0;
    String stMessage = "";
    lcd.clear();
    for (int i = 0; i < length; i++)
    {
        Serial.print((char)message[i]);
        stMessage += (char)message[i];
        dem++;
        if (dem == 20)
        {
            dem = 0;
            lcd.setCursor(0, dem1);
            lcd.print(stMessage);
            dem1++;
            stMessage = "";
            if (dem1 == 4)
            {
                dem1 = 0;
                break;
            }
        }
    }
    if (length < 20)
    {
        lcd.setCursor(0, 1);
        lcd.print(stMessage);
    }
}

void setup()
{
    Serial.begin(115200);

    WIFIConnect();
    client.setServer(MQTTServer, Port);
    client.setCallback(callback);
    lcd.init();
    lcd.backlight();
}

void loop()
{
    delay(10);
    if (!client.connected())
    {
        MQTT_Reconnect();
    }
    client.loop();
}
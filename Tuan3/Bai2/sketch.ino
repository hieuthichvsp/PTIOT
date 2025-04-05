#include <Keypad.h>
#include <Wire.h>
#include <PubSubClient.h>
#include <WiFi.h>

#define row_num 4
#define col_num 4

const char *MQTTServer = "broker.emqx.io";
const char *MQTT_Topic = "22004266/KEYPAD";
const char *MQTT_ID = "8479cc69-d8f4-4385-9c23-c1ddbe5da6c8";
int Port = 1883;
String str = "";

int buzzer = 23;
char keys[row_num][col_num] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}};

byte row_pins[row_num] = {22, 21, 19, 18};
byte col_pins[col_num] = {5, 17, 16, 4};

String password = "11111";
String input = "";

Keypad mykeypad = Keypad(makeKeymap(keys), row_pins, col_pins, row_num, col_num);

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
    String stMessage;

    for (int i = 0; i < length; i++)
    {
        Serial.print((char)message[i]);
        stMessage += (char)message[i];
    }
    str = stMessage;
    Serial.println();
}

void setup()
{
    Serial.begin(115200);
    WIFIConnect();
    client.setServer(MQTTServer, Port);
    client.setCallback(callback);
    pinMode(buzzer, OUTPUT); // Đảm bảo chân buzzer được thiết lập là OUTPUT
}

void loop()
{
    char key = mykeypad.getKey();
    delay(10);

    if (!client.connected())
    {
        MQTT_Reconnect();
    }
    client.loop();
    Serial.println(key);

    if (key)
    {
        // if '#' pressed, check password
        if (key == '#')
        {
            Serial.println();
            if (input == str)
            {
                digitalWrite(buzzer, HIGH);
                delay(500);
                digitalWrite(buzzer, LOW);
            }
            else
            {
                for (int i = 0; i < 3; i++)
                {
                    digitalWrite(buzzer, HIGH);
                    delay(500);
                    digitalWrite(buzzer, LOW);
                    delay(500);
                }
            }
            input = ""; // <-- reset input ở đây
        }
        else if (key == '*')
        {
            input = ""; // Reset input nếu nhấn *
        }
        else
        {
            input.concat(key); // Thêm ký tự vào input
            Serial.print(key);
        }
    }
}

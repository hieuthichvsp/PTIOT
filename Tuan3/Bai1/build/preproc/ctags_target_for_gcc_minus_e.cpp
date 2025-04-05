# 1 "c:\\Users\\lolhi\\Documents\\Vsc_arduino\\PTIOT\\Tuan3\\Bai1\\sketch.ino"
# 2 "c:\\Users\\lolhi\\Documents\\Vsc_arduino\\PTIOT\\Tuan3\\Bai1\\sketch.ino" 2
# 3 "c:\\Users\\lolhi\\Documents\\Vsc_arduino\\PTIOT\\Tuan3\\Bai1\\sketch.ino" 2
# 4 "c:\\Users\\lolhi\\Documents\\Vsc_arduino\\PTIOT\\Tuan3\\Bai1\\sketch.ino" 2

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
    Serial0.println("Connecting to SSID: Wokwi-GUEST");
    WiFi.begin("Hieu", "hieudzvd");
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial0.print(".");
    }
    Serial0.println("");
    Serial0.print("WiFi connected");
    Serial0.print(", IP address: ");
    Serial0.println(WiFi.localIP());
}

void MQTT_Reconnect()
{
    while (!client.connected())
    {
        if (client.connect(MQTT_ID))
        {
            Serial0.print("MQTT Topic: ");
            Serial0.print(MQTT_Topic);
            Serial0.print(" connected");
            client.subscribe(MQTT_Topic);
            Serial0.println("");
        }
        else
        {
            Serial0.print("failed, rc=");
            Serial0.print(client.state());
            Serial0.println(" try again in 5 seconds");
            delay(5000);
        }
    }
}

void callback(char *topic, byte *message, unsigned int length)
{
    Serial0.print("Message arrived on topic: ");
    Serial0.println(topic);
    Serial0.print("Message: ");
    String stMessage;
    DynamicJsonBuffer jsonBuffer;

    for (int i = 0; i < length; i++)
    {
        Serial0.print((char)message[i]);
        stMessage += (char)message[i];
    }
    JsonObject &data = jsonBuffer.parseObject(stMessage);
    for (int i = 0; i < 3; i++)
    {
        String key = String(i + 1);
        timeLed[i] = data["led"][i][key.c_str()].as<int>() * 1000;
        Serial0.println(timeLed[i]);
    }
    flag = 1;
    client.publish("22004266/RETURN", "Da nhan");
}

void setup()
{
    Serial0.begin(115200);
    WIFIConnect();
    client.setServer(MQTTServer, Port);
    client.setCallback(callback);
    for (int i = 0; i < 3; i++)
    {
        pinMode(ledPin[i], 0x03);
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
            digitalWrite(ledPin[0], 0x1);
            delay(10);
            if (temp == timeLed[0])
                flag = 2;
        }
        else
        {
            digitalWrite(ledPin[0], 0x0);
        }
    }
    // Yellow Led
    temp = 0;
    while (temp + 10 < timeLed[1])
    {
        if (flag == 2)
        {
            client.loop();
            digitalWrite(ledPin[1], 0x1);
            delay(10);
            if (temp == timeLed[1])
                flag = 3;
        }
        else
        {
            digitalWrite(ledPin[1], 0x0);
        }
    }
    // Red Led
    temp = 0;
    while (temp + 10 < timeLed[2])
    {
        if (flag == 3)
        {
            client.loop();
            digitalWrite(ledPin[2], 0x1);
            delay(10);
            if (temp == timeLed[2])
                flag = 1;
        }
        else
        {
            digitalWrite(ledPin[2], 0x0);
        }
    }
}

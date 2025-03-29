# 1 "c:\\Users\\lolhi\\Documents\\Vsc_arduino\\PTIOT\\Tuan1\\sketch.ino"
# 2 "c:\\Users\\lolhi\\Documents\\Vsc_arduino\\PTIOT\\Tuan1\\sketch.ino" 2
# 3 "c:\\Users\\lolhi\\Documents\\Vsc_arduino\\PTIOT\\Tuan1\\sketch.ino" 2
# 4 "c:\\Users\\lolhi\\Documents\\Vsc_arduino\\PTIOT\\Tuan1\\sketch.ino" 2

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

void TuanTu()
{
    for (int i = 0; i < 4; i++)
    {
        digitalWrite(ledPin[i], 0x1);
        delay(500);
        digitalWrite(ledPin[i], 0x0);
        delay(500);
    }
}

void NgauNhien()
{
    int i = random(0, 4);
    digitalWrite(ledPin[i], 0x1);
    delay(500);
    digitalWrite(ledPin[i], 0x0);
    delay(500);
}

void CungLuc()
{
    for (int i = 0; i < 4; i++)
    {
        digitalWrite(ledPin[i], 0x1);
    }
    delay(500);
    for (int i = 0; i < 4; i++)
    {
        digitalWrite(ledPin[i], 0x0);
    }
    delay(500);
}

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

    for (int i = 0; i < length; i++)
    {
        Serial0.print((char)message[i]);
        stMessage += (char)message[i];
    }
    Serial0.println();
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
            digitalWrite(ledPin[i], 0x1);
        }
    }
    else if (stMessage == "tat")
    {
        flag = 0;
        for (int i = 0; i < 4; i++)
        {
            digitalWrite(ledPin[i], 0x0);
        }
    }
}

void setup()
{
    Serial0.begin(115200);

    WIFIConnect();
    client.setServer(MQTTServer, Port);
    client.setCallback(callback);
    for (int i = 0; i < 4; i++)
    {
        pinMode(ledPin[i], 0x03);
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

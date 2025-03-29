# 1 "c:\\Users\\lolhi\\Documents\\Vsc_arduino\\PTIOT\\Tuan2\\Bai2\\sketch.ino"
# 2 "c:\\Users\\lolhi\\Documents\\Vsc_arduino\\PTIOT\\Tuan2\\Bai2\\sketch.ino" 2
# 3 "c:\\Users\\lolhi\\Documents\\Vsc_arduino\\PTIOT\\Tuan2\\Bai2\\sketch.ino" 2
# 4 "c:\\Users\\lolhi\\Documents\\Vsc_arduino\\PTIOT\\Tuan2\\Bai2\\sketch.ino" 2

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
    Serial0.println("Connecting to SSID: Wokwi-GUEST");
    WiFi.begin("Wokwi-GUEST", "");
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
    int dem = 0;
    int dem1 = 0;
    String stMessage = "";
    lcd.clear();
    for (int i = 0; i < length; i++)
    {
        Serial0.print((char)message[i]);
        stMessage += (char)message[i];
        dem++;
        if (dem == 20)
        {
            dem = 0;
            lcd.setCursor(0, dem1);
            lcd.print(stMessage);
            dem1++;
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
    Serial0.begin(115200);

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

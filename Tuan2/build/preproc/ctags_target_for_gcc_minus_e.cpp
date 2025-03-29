# 1 "c:\\Users\\lolhi\\Documents\\Vsc_arduino\\PTIOT\\Tuan2\\sketch.ino"
# 2 "c:\\Users\\lolhi\\Documents\\Vsc_arduino\\PTIOT\\Tuan2\\sketch.ino" 2
# 3 "c:\\Users\\lolhi\\Documents\\Vsc_arduino\\PTIOT\\Tuan2\\sketch.ino" 2
# 4 "c:\\Users\\lolhi\\Documents\\Vsc_arduino\\PTIOT\\Tuan2\\sketch.ino" 2
# 5 "c:\\Users\\lolhi\\Documents\\Vsc_arduino\\PTIOT\\Tuan2\\sketch.ino" 2
# 6 "c:\\Users\\lolhi\\Documents\\Vsc_arduino\\PTIOT\\Tuan2\\sketch.ino" 2

JsonDocument doc;


DHT dht(21, DHT22);

const char *MQTTServer = "broker.emqx.io";
const char *MQTT_Topic = "22004266/Data";

// Tạo ID ngẫu nhiên tại: https://www.guidgen.com/
const char *MQTT_ID = "23109811-5b67-4e56-aa4d-1b26fa14451c";
int Port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

void WIFIConnect()
{
    dht.begin();
    Serial0.println("Connecting to SSID: Wokwi-GUEST");
    //   WiFi.begin("Wokwi-GUEST", "");
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
    Serial0.begin(115200);
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
    Serial0.print("Temperature: ");
    Serial0.print(temp);
    Serial0.print("°C, Humidity: ");
    Serial0.print(humi);
    Serial0.println("%");
    String jsonString = doc.as<String>();
    client.publish(MQTT_Topic, jsonString.c_str());
    client.loop();
}

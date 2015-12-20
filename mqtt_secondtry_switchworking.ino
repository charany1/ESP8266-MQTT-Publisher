
//This sketch is used to publish dat from esp8266 device to a public MQTT broker.
#include <PubSubClient.h>
#include <ESP8266WiFi.h>
const char* ssid = "SSID";
const char* password = "Wifipassword";
char* topic = "door/status";
char* server = "broker.hivemq.com";

int Switch = 0;
int Led = 5;
int SwitchPosition,PreviousPosition;

WiFiClient wifiClient;

void callback(char* topic, byte* payload, unsigned int length) {
// handle message arrived
}

PubSubClient client(server, 1883, callback, wifiClient);

String macToStr(const uint8_t* mac)
{
  String result;
for (int i = 0; i < 6; ++i) {
    result += String(mac[i], 16);
if (i < 5)
      result += ':';
  }
return result;
}
void setup() {
  Serial.begin(115200);
  pinMode(Led,OUTPUT);
  pinMode(Switch,INPUT_PULLUP);
  delay(10);
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
while (WiFi.status() != WL_CONNECTED) {
delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
// Generate client name based on MAC address and last 8 bits of microsecond counter
  String clientName;
  clientName += "esp8266-";
uint8_t mac[6];
  WiFi.macAddress(mac);
  clientName += macToStr(mac);
  clientName += "-";
  clientName += String(micros() & 0xff, 16);
  Serial.print("Connecting to ");
  Serial.print(server);
  Serial.print(" as ");
  Serial.println(clientName);
if (client.connect((char*) clientName.c_str())) {
    Serial.println("Connected to MQTT broker");
    Serial.print("Topic is: ");
    Serial.println(topic);
if (client.publish(topic, "hello from ESP8266")) {
      Serial.println("Publish ok");
    }
else {
      Serial.println("Publish failed");
    }
  }
else {
    Serial.println("MQTT connect failed");
    Serial.println("Will reset and try again...");
abort();
  }
  PreviousPosition = HIGH;
}
void loop() 
{
  static int counter = 0;
  

  SwitchPosition = digitalRead(Switch);
  if (PreviousPosition != SwitchPosition)
  {  
    String payload = "OPEN";
    digitalWrite(Led,LOW);
    if (SwitchPosition == LOW)
    {
      payload = "CLOSE";
      digitalWrite(Led,HIGH);
    }
    if (client.connected()){
        Serial.print("Sending payload: ");
        Serial.println(payload);
    if (client.publish(topic, (char*) payload.c_str())) {
          Serial.println("Publish ok");
        }
    else {
          Serial.println("Publish failed");
        }
      }
    ++counter;
    PreviousPosition = SwitchPosition;
    delay(2000);
  }
}

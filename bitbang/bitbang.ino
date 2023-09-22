#define SCK 18 //Clk
#define MOSI 23 //Data
#define SS 5 // Pulse seperating low and high speed packets

#include <ArduinoMqttClient.h>
#include <WiFi.h>
#include "ArduinoJson.h"
#include "arduino_secrets.h"

char ssid[] = SECRET_SSID; // Your WIFI Network SSID
char password[] = SECRET_PASS; // your WIFI Network password
char broker[] = SECRET_BROKER; // Your MQQT broker hostname
char brokeruser[] = SECRET_BROKERUSER; // Your MQQT broker username
char brokerpass[] = SECRET_BROKERPASS; // Your MQQT broker password
int brokerport = 1883;
int displayDelay = 34;

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

const char topic[]  = "AstraGTEDials";

byte fuzz;

void setup() {
  Serial.begin(115200);

  pinMode(SS, OUTPUT);
  pinMode(SCK, OUTPUT);
  pinMode(MOSI, OUTPUT);

  digitalWrite(MOSI, HIGH);
  digitalWrite(SCK, HIGH);
  digitalWrite(SS, HIGH);
  delay(100);

  digitalWrite(MOSI, LOW);
  digitalWrite(SCK, LOW);
  digitalWrite(SS, LOW);

  initWiFi();

  mqttClient.setUsernamePassword(brokeruser, brokerpass);
  if (!mqttClient.connect(broker, brokerport)) {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient.connectError());
    while (1);
  }
  mqttClient.subscribe(topic);
  Serial.println("You're connected to the MQTT broker!");

  // set the message receive callback
  mqttClient.onMessage(onMqttMessage);
}


void processPayload(String payload) {
  Serial.println("Processing payload");
  // bitBangDatflipByte(fuzz)); // digit 3 and MPH/KMH
  bitBangData(0x00); // digit 3 and MPH/KMH
  bitBangData(0xFF); // digit 2 and 1st 4 segs of revs -- also something on speedo?
  bitBangData(0xFF); // next 8 segs of revs
  bitBangData(0xFF); // next 8 segs of revs
  bitBangData(0xFF); // last 8 segs of revs
  bitBangData(0xFF); // revs colour and red bits (0XFF for Red bits on)
  bitBangData(0xFF); // digit 1 and 2 of MPH/KPH. All 4 of left indicators -
  // bitBangData(flipByte(doc["battery"]); // Oil segments, battery alarm (0x15 == battery alarm off, 0x13 == battery alarm on)
  bitBangData(0xFF); // Oil segments, battery alarm (0x15 == battery alarm off, 0x13 == battery alarm on)
  bitBangData(0xFF); // battery segments, temp alarm
  bitBangData(0xFF); // temp segments, fuel alarm
  bitBangData(0xFF); // fuel segments
  bitBangData(0xFF); // fuel segment end
  digitalWrite(SS, HIGH);
  delayMicroseconds(2);
  digitalWrite(SS, LOW);
}



void onMqttMessage(int messageSize) {
  String payload = "";
  char character;
  // Turn stream into string
  while (mqttClient.available()) {
     character = (char)mqttClient.read();
     payload.concat(character);
  }
  Serial.println(payload); // {temp:"24", battery:"99"}

  StaticJsonDocument<128> doc;
  DeserializationError error = deserializeJson(doc, payload);

  if (error) {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
    return;
  }

  const char* battery = doc["battery"]; // "50"
  const char* temp = doc["temp"]; // "24"
  Serial.println(battery);
  Serial.println(temp);
  processPayload(payload);
}


void loop() {
  mqttClient.poll();
}



byte bitBangData(byte _send) {
  for(int i=0; i<8; i++) {
    digitalWrite(MOSI, bitRead(_send, i));
    delayMicroseconds(1);
    digitalWrite(SCK, LOW);
    delayMicroseconds(1);
    digitalWrite(SCK, HIGH);
    // Pads this so each bit is approx 34ms
    delayMicroseconds(displayDelay); 
  }
  digitalWrite(MOSI, LOW);
  return false;
}

void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
}


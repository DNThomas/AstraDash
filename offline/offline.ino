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
}


void processPayload() {
  Serial.println("Processing payload:");
  Serial.print(fuzz);
  Serial.print(" ");
  Serial.print(fuzz, BIN);
  bitBangData(0x00); // digit 3 and MPH/KMH
  bitBangData(0xFF); // digit 2 and 1st 4 segs of revs -- also something on speedo?
  bitBangData(0xFF); // next 8 segs of revs
  bitBangData(0xFF); // next 8 segs of revs
  bitBangData(0xFF); // last 8 segs of revs
  bitBangData(0xFF); // revs colour and red bits (0XFF for Red bits on)
  bitBangData(0xFF); // digit 1 and 2 of MPH/KPH. All 4 of left indicators -
  bitBangData(0xFF); // Oil segments, battery alarm (0x15 == battery alarm off, 0x13 == battery alarm on)
  bitBangData(fuzz); // battery segments (6), temp segments(2), temp alarm (1)
/*
  - 32 - just one segment = 1,0,1
  - 1 two sections and light = 2,1
  - 3 three sections and light = 3,0,1
  - 7 4 secotions and light = 4,0,1
  - 15 - all lit up = 6,0,1
  - 31 - 
  - 34 - 
  - 35 - 
  - 39 - 
  - 47 5?
  - 63 - 6,1,1
  - 64 light off
  - 78 - everything = 
  - 95 - light came on = 
99
103
110
126
134
143
158
161
163, 166, 225 = temp light, 255 = all on and temp light
30
*/
  bitBangData(0xFF); // temp segments, fuel alarm
  bitBangData(0xFF); // fuel segments
  bitBangData(0xFF); // fuel segment end
  digitalWrite(SS, HIGH);
  delayMicroseconds(2);
  digitalWrite(SS, LOW);
  Serial.println();
  fuzz++;
}


void loop() {
  processPayload();
  delay(3000);
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


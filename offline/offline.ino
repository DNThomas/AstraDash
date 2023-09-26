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


void processPayload(bool flip) {
  byte data = 0b10000000;
  if (flip) {
    Serial.print("turning all on");
    data = 0b00000000;
  }
  Serial.println("Processing payload:");
  Serial.print(data);
  bitBangData(data); // digit 3 and MPH/KMH
/*
              || ignore
                | - middle
                 | - TBD
                  | - TBD
                   | - TBD
                    | - TBD
                     | - TBD 
                      | - TBD
                       | - Temp warning

*/
  bitBangData(data); // digit 2 and 1st 4 segs of revs -- also something on speedo?
/*
              || ignore
                | - 6
                 | - TBD
                  | - TBD
                   | - 16k rpm
                    | - TBD
                     | - 12k rpm
                      | - 10k rpm
                       | - 6k rpm

*/
  bitBangData(data); // next 8 segs of revs
/*
              || ignore
                | - 22
                 | - TBD
                  | - TBD
                   | - 32k rpm
                    | - TBD
                     | - TBD
                      | - 25k rpm
                       | - 24k rpm

*/
  bitBangData(data); // next 8 segs of revs
/*
              || ignore
                | - 38
                 | - TBD
                  | - TBD
                   | - 49k rpm
                    | - TBD
                     | - 42k rpm
                      | - TBD
                       | - 42k rpm

*/
  bitBangData(data); // last 8 segs of revs
/*
              || ignore
                | - 50
                 | - 62
                  | - TBD 
                   | - TBD
                    | - TBD
                     | - TBD
                      | - TBD
                       | - 80k rpm

*/
  bitBangData(data); // revs colour and red bits (0XFF for Red bits on)
/*
              || ignore
                | - TBD
                 | - TBD
                  | - TBD
                   | - Red line above warning
                    | - TBD
                     | - First Red line?
                      | - TBD
                       | - TBD

*/
  bitBangData(data); // digit 1 and 2 of MPH/KPH. All 4 of left indicators -
/*
              || ignore
                | - TBD
                 | - TBD
                  | - digit 1 > top middle, bottom left, middle middle, bottom middle
                   | - digit 1 > top right
                    | - digit 1 > bottom right
                     | - digit 2 > bottom middle
                      | - digit 2 > bottom left
                       | - digit 2 > middle section, oil warning light?

*/
  bitBangData(data); // Oil segments, battery alarm (0x15 == battery alarm off, 0x13 == battery alarm on) -- first bit of battery
/*
              || ignore
                | - 1st bit of battery
                 | - battery alarm
                  | - final section of oil
                   | - 4th section of oil
                    | - 3rd section of oil
                     | - 2nd section of oil
                      | - 1st section of oil
                       | - oil warning?

*/
  bitBangData(data); // battery segments (6), temp segments(2), temp alarm (1) - 
/*
              || ignore
                | - 2nd bit of temp
                 | - 1st bit of temp
                  | - temp light
                   | - 6th bit of battery
                    | - 5th bit of battery
                     | - 4th bit of battery
                      | - 3rd bit of battery
                       | - 2nd bit of battery

*/
  bitBangData(data); // temp segments, fuel alarm
/*
              || ignore
                | - 2nd bit of temp
                 | - 1st bit of temp
                  | - TBD
                   | - fuel alarm
                    | - Final red bit of temp
                     | - 5th bit of temp
                      | - 4th bit of temp
                       | - 3rd bit of temp

*/
  bitBangData(data); // fuel segments
/*
              || ignore
                | - 2nd bit of fuel or 7th?
                 | - TBD
                  | - 1st bit
                   | - 6th bit of fuel
                    | - 5th bit of fuiel
                     | - 4th bit of Fuel?
                      | - 3rd bit of Fuel?  Hard to tell
                       | - 2nd bit of Fuel?  Hard to tell

*/
  bitBangData(data); // fuel segment end
/*
              || ignore
                | - penultimate bit of fuel?
                 | - final bit of fuel
                  | - 7th bit of fuel
                   | - TBD
                    | - TBD
                     | - TBD?
                      | - TBD
                       | - final bit of fuel, maybe 8?

*/
  digitalWrite(SS, HIGH);
  delayMicroseconds(2);
  digitalWrite(SS, LOW);
  Serial.println();
}


void loop() {
  processPayload(false);
  delay(1000);
/*
  processPayload(true);
  delay(1000);
*/
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


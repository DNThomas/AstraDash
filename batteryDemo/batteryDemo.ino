#define SCK 18 //Clk
#define MOSI 23 //Data
#define SS 5 // Pulse seperating low and high speed packets

#include <ArduinoMqttClient.h>
int displayDelay = 34;
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

void loop () {
  int i = 0;
  // battery alarm on
  processPayload(0b01000000, 0b00000000);
  delay(1000);

  // first battery gauge on
  processPayload(0b10000000, 0b00000000);
  delay(1000);

  // 2nd battery gauge on
  processPayload(0b10000000, 0b00000001);
  delay(1000);

  // 3rd battery gauge on
  processPayload(0b10000000, 0b00000011);
  delay(1000);

  // 4th battery gauge on
  processPayload(0b10000000, 0b00000111);
  delay(1000);

  // 5th battery gauge on
  processPayload(0b10000000, 0b00001111);
  delay(1000);

  // 6th battery gauge on
  processPayload(0b10000000, 0b00011111);
  delay(1000);

  i++;
}

void processPayload(byte data, byte datatwo) {
  Serial.print("Processing payload: ");
  Serial.println(data);
  Serial.println(datatwo);
  byte nulldata = 0b00000000;
  bitBangData(nulldata); // digit 3 and MPH/KMH
  bitBangData(nulldata); // digit 2 and 1st 4 segs of revs -- also something on speedo?
  bitBangData(nulldata); // next 8 segs of revs
  bitBangData(nulldata); // next 8 segs of revs
  bitBangData(nulldata); // last 8 segs of revs
  bitBangData(nulldata); // revs colour and red bits (0XFF for Red bits on)
  bitBangData(0b10000000); // digit 1 and 2 of MPH/KPH. All 4 of left indicators -
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
  bitBangData(datatwo); // battery segments (6), temp segments(2), temp alarm (1) - 
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
  bitBangData(nulldata); // temp segments, fuel alarm
  bitBangData(nulldata); // fuel segments
  bitBangData(nulldata); // fuel segment end
  digitalWrite(SS, HIGH);
  delayMicroseconds(2);
  digitalWrite(SS, LOW);
  Serial.println();
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


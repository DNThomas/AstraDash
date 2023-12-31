#define SCK 18 //Clk
#define MOSI 23 //Data
#define SS 5 // Pulse seperating low and high speed packets

#include <ArduinoMqttClient.h>
#include <WiFi.h>
#include "ArduinoJson.h"
#include "arduino_secrets.h"
#include "valuesToBinary.h"

char ssid[] = SECRET_SSID; // Your WIFI Network SSID
char password[] = SECRET_PASS; // your WIFI Network password
char broker[] = SECRET_BROKER; // Your MQQT broker hostname
char brokeruser[] = SECRET_BROKERUSER; // Your MQQT broker username
char brokerpass[] = SECRET_BROKERPASS; // Your MQQT broker password
int brokerport = 1883;

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

const char topic[]  = "AstraGTEDials";
unsigned long previousMillis = 0;
unsigned long interval = 30000;

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


void onMqttMessage(int messageSize) {
  String payload = "";
  char character;
  // Turn stream into string
  while (mqttClient.available()) {
     character = (char)mqttClient.read();
     payload.concat(character);
  }
  Serial.println(payload);

  StaticJsonDocument<200> doc;
  DeserializationError error = deserializeJson(doc, payload);

  if (error) {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
    return;
  }

  // Oil Temp
  const char* incomingOilTemp = doc["oilTemp"]; // temporary example value that Home Assistant sent us.
  int numberOfSegments = sizeof(oilTemp) / sizeof (oilTemp[0]); // Get size, can't do this in function
  setBitsBasedOnInput(incomingOilTemp, oilTempCeiling, oilTempFloor, oilTemp, numberOfSegments); // set the oil temp bits

  // Battery Level
  const char* incomingBattery = doc["battery"];
  numberOfSegments = sizeof(battery) / sizeof (battery[0]); // Get size, can't do this in function
  setBitsBasedOnInput(incomingBattery, batteryCeiling, batteryFloor, battery, numberOfSegments); // set the battery level

  // Coolant Temp
  const char* incomingCoolantTemp = doc["coolantTemp"];
  numberOfSegments = sizeof(coolantTemp) / sizeof (coolantTemp[0]); // Get size, can't do this in function
  setBitsBasedOnInput(incomingCoolantTemp, coolantTempCeiling, coolantTempFloor, coolantTemp, numberOfSegments); // set the coolant temp level

  // Fuel Level
  const char* incomingFuelLevel = doc["fuelLevel"];
  numberOfSegments = sizeof(fuelLevel) / sizeof (fuelLevel[0]); // Get size, can't do this in function
  setBitsBasedOnInput(incomingFuelLevel, fuelLevelCeiling, fuelLevelFloor, fuelLevel, numberOfSegments); // set the coolant temp level

  // RPM
  const char* incomingRPM = doc["rpm"];
  numberOfSegments = sizeof(rpm) / sizeof (rpm[0]); // Get size, can't do this in function
  setBitsBasedOnInput(incomingRPM, rpmCeiling, rpmFloor, rpm, numberOfSegments); // set the coolant temp level

  // Speedo
  // First to reset
  int numberOfDigits = 20; // 20 possible drawable things to reset
  int digCount = 0;
  while (digCount < numberOfDigits) {
    integerToByteWrite(digits[digCount], 0);
    digCount++;
  }

  // Now we have reset we can write the digits
  const char* incomingSpeedo = doc["speedo"];
  String stringValue = String(incomingSpeedo);
  int val = stringValue.toInt();
  int* speedoBits = digitthree[val];
//  Serial.print("Setting the Speedo digits: ");
//  Serial.println(val);
  int numberOfParts = sizeof(digitthree[val]) / sizeof(digitthree[val][0]);
  // TODO: There is some weird bug here where numberOfParts is the wrong size..
  // It doesn't appear to break anything but it needs looking at when I'm better with C++
//  Serial.print("Size");
  Serial.println(numberOfParts);
  int segmentCount = 0;
  while (segmentCount < numberOfParts) {
    Serial.println(speedoBits[segmentCount]);
    integerToByteWrite(speedoBits[segmentCount], 1);
    segmentCount++;
  }

  // All data collected we can draw the dash
  int m = 0;
  Serial.println("Updating Dash");
  while (m < 12){ // go through all the bytes
    for(int i=0; i<8; i++) { // The iteration is working right
//      Serial.print("BIN: ");
//      Serial.println(bytes[0], BIN);
      int foo = bitRead(bytes[m], i);
/*
      Serial.print("Byte: ");
      Serial.print(m);
      Serial.print(" > Bit: ");
      Serial.print(i);
      Serial.print(" -- value: ");
      Serial.println(foo); // this value isn't being set?
*/
      digitalWrite(MOSI, bitRead(bytes[m], i));
      delayMicroseconds(1);
      digitalWrite(SCK, LOW);
      delayMicroseconds(1);
      digitalWrite(SCK, HIGH);
    }

    digitalWrite(MOSI, LOW);
    m++; // go to next byte
  }
  digitalWrite(SS, HIGH);
  delayMicroseconds(2);
  digitalWrite(SS, LOW);
}

void loop() {
  mqttClient.poll();

unsigned long currentMillis = millis();
  if (currentMillis => 86400000){
    ESP.restart();
  } 

// if WiFi is down, try reconnecting
if ((WiFi.status() != WL_CONNECTED) && (currentMillis - previousMillis >=interval)) {
  Serial.print(millis());
  Serial.println("Reconnecting to WiFi...");
  WiFi.disconnect();
  WiFi.reconnect();
  previousMillis = currentMillis;
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


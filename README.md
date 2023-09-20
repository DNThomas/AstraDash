# ESP32 Astra GTE Dash
Arduino code to use a ESP32 to control an Astra GTE Dashboard display.

## Initial wiring
1. Disconnect the blue connector from the main control board to the display board
![Disconnecting the blue connector on the Astra GTE Dials](https://github.com/JohnMcLear/AstraDash/assets/220864/3600ab1a-57b3-4267-b3ec-5098f7bfbefa)

1. Connect Dupont wires from pins 3,4,5,6,7 to ESP32 as per "Wiring" instructions
![Connecting the ESP32 to the Astra GTE Dials](https://github.com/JohnMcLear/AstraDash/assets/220864/44b5a965-aa06-49ab-9637-cea280dbe507)


## Wiring: GTE Dials pin to Arduino

3 VCC <> Direct from USB 5v feed

4 GND <> Direct from Ground

5 CLK <> SCK 18

6 MOSI/DATA <> MOSI 23

7 SS <> SS 5


## Installation
1. Clone the repo: ``git clone https://github.com/JohnMcLear/AstraDash.git && cd AstraDash``
1. ``curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | sh``
1. ``bin/arduino-cli config init``
1. ``bin/arduino-cli core update-index --additional-urls https://arduino.esp8266.com/stable/package_esp8266com_index.json``
1. ``bin/arduino-cli core update-index && bin/arduino-cli board listall && bin/arduino-cli core install esp32:esp32``

## Configure WiFi and MQQT
Create ``bitbang/arduino_secrets.h`` and paste the below in editing the values accordingly:
```
#define SECRET_SSID "YourSSIDHere"
#define SECRET_PASS "YourPassHere"
#define SECRET_BROKER "YourMQQTBrokerHostnameHere"
#define SECRET_BROKERUSER "YourMQQTBrokerUsernameHere"
#define SECRET_BROKERPASS "YourMQQTBrokerPasswordHere"
```

## Compile
``bin/arduino-cli compile --fqbn esp32:esp32:nodemcu-32s bitbang``

## Run
Note: Change ttyUSB1 to whatever port you are targeting.  ``sudo dmesg | grep tty`` to discover port.

``bin/arduino-cli upload -p /dev/ttyUSB1 --fqbn esp32:esp32:nodemcu-32s bitbang``

Or for those who will make lots of changes...

## Compile and Run (assumes /dev/ttyUSB0)
``./test.sh``

## Notes
1. We don't use MISO as it response from Dash to Arduino.
1. Btw .. your easiest way would be to use old skool screen paging.  Have a 12 byte block of memory assigned to the screen. Put the screen refresh on a timed interrupt and have it say every 100ms. Then just have a loop that waits for commands from serial or websocket that updates the bitmap


## TODO
 - [ ] Do one gauge so I have working code that shows a certain value
 - [ ] Mask Bits so simple values can be written.
 - [ ] Design data structure IE if I want to write low battery level to fuel gauge I want to write something like "fuel,0"
 - [ ] Figure out a way to interact with the ESP32 to update the value of a given item



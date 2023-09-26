# ESP32 Astra GTE Dash
Arduino code to use a ESP32 to control an Astra GTE Dashboard display.

## Initial wiring
1. Disconnect the blue connector from the main control board to the display board
![Disconnecting the blue connector on the Astra GTE Dials](https://github.com/JohnMcLear/AstraDash/assets/220864/3600ab1a-57b3-4267-b3ec-5098f7bfbefa)

1. Connect Dupont wires from pins 3,4,5,6,7 to ESP32 as per "Wiring" instructions
![Connecting the ESP32 to the Astra GTE Dials (Astra Dials View)](https://github.com/JohnMcLear/AstraDash/assets/220864/44b5a965-aa06-49ab-9637-cea280dbe507)

![Connecting the ESP32 to the Astra GTE Dials (ESP view)](https://github.com/JohnMcLear/AstraDash/assets/220864/fbfee946-2f15-4cee-b03e-1722e2407713)


## Wiring: GTE Dials pin to Arduino

3 VCC <> Direct from USB 5v feed

4 GND <> Direct from Ground

5 CLK <> SCK 18

6 MOSI/DATA <> MOSI 23

## Installation
1. Clone the repo: ``git clone https://github.com/JohnMcLear/AstraDash.git && cd AstraDash``
1. ``curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | sh``
1. ``bin/arduino-cli config init``
1. ``bin/arduino-cli core update-index --additional-urls https://arduino.esp8266.com/stable/package_esp8266com_index.json``
1. ``bin/arduino-cli core update-index && bin/arduino-cli board listall && bin/arduino-cli core install esp32:esp32``

## Configure WiFi and MQQT
Create ``AstraDashHomeAssistant/arduino_secrets.h`` and paste the below in editing the values accordingly:
```
#define SECRET_SSID "YourSSIDHere"
#define SECRET_PASS "YourPassHere"
#define SECRET_BROKER "YourMQQTBrokerHostnameHere"
#define SECRET_BROKERUSER "YourMQQTBrokerUsernameHere"
#define SECRET_BROKERPASS "YourMQQTBrokerPasswordHere"
```

## Compile
``bin/arduino-cli compile --fqbn esp32:esp32:nodemcu-32s AstraDashHomeAssistant``

## Run
Note: Change ttyUSB1 to whatever port you are targeting.  ``sudo dmesg | grep tty`` to discover port.

``bin/arduino-cli upload -p /dev/ttyUSB1 --fqbn esp32:esp32:nodemcu-32s AstraDashHomeAssistant``

Or for those who will make lots of changes...

## Compile and Run (assumes /dev/ttyUSB0)
``./test.sh``

## Testing the Display
``./testDisplay.sh``

## Configuring Home Assistant
Create the automation with the following Yaml

```
alias: Every second Publish to Astra GTE Dials
description: ""
trigger:
  - platform: time_pattern
    seconds: /5
condition: []
action:
  - service: mqtt.publish
    data:
      qos: 0
      retain: false
      topic: AstraGTEDials
      payload: "{temp:\"24\", battery:\"99\"}"
mode: single
```


## Notes
1. We don't use MISO as it response from Dash to Arduino.
1. Btw .. your easiest way would be to use old skool screen paging.  Have a 12 byte block of memory assigned to the screen. Put the screen refresh on a timed interrupt and have it say every 100ms. Then just have a loop that waits for commands from serial or websocket that updates the bitmap
1. The following are NOT supported: Hazards, Lights, Indicators, Choke, Parking Brake ..   Basically anything on the bottom row.  These could however be supported with additional relay control board.

## TODO
 - [ ] Do one gauge so I have working code that shows a certain value
 - [ ] Mask Bits so simple values can be written.
 - [ ] Design data structure IE if I want to write low battery level to fuel gauge I want to write something like "fuel:0"
   Currently we write 12 values, on an mqqt update we only know 1 of those 12 values so we'd need to store the previous value and look it up.
   There are two approaches for passing Data..
    1. Build bitmap in HA and pass it to the ESP32
    1. Pass integer values from HA and have the ESP32 build the bitmap
 -- I guess either way I want something that can support "oil:20" or "battery:40" where 20/40 are %..  I also want to be able to do:

## Output Designation
 - [ ] Oil Temp > Outdoor Temp Degrees C (-5 > 40)
 - [ ] Oil Temp Warning > Outdoor Temp < 0
 - [ ] Battery Level > Home Storage Battery level %
 - [ ] Battery Level Warning > Battery Level < 10%
 - [ ] Coolant Temp > Indoor temp C (-5 > 40)
 - [ ] Coolant Temp Warning > Indoor Temp < 15 degs
 - [ ] Fuel Level > Electric car charge %
 - [ ] Fuel Level warning > Electric car charge < 20%
 - [ ] Speedo > How many tasks I have to do today (see input_number.active_todoist_jobs_today)
 - [ ] RPM > Solar generation 0 > 7000

## Brain dump on driving the display.
Currently the display is segmented by 12 x 256 values(8 bit) resulting in ~3000 potential states / states.

Thankfully we know how to address each segment so our next step is to go through each segment and pass in various 8 bit values to find a value that changes the state we want.

For example; we can go through all of the potential values on the fuel gauge and discover 

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
      payload: >-
        { oilTemp:"{{ states('sensor.outdoor_temperature') }}",
        battery:"{{states('sensor.solarbattery') }}",
        coolantTemp:"{{state_attr('climate.house', 'current_temperature') }}",
        fuelLevel:"{{states('sensor.leaf1ljpi_charge')}}",
        rpm:"{{states('sensor.solargeneration')}}",
        speedo:"{{states('input_number.active_todoist_jobs_today')}}" }
mode: single
```


## Notes
1. The following are NOT supported: Hazards, Lights, Indicators, Choke, Parking Brake ..   Basically anything on the bottom row.  These could however be supported with additional relay control board.

## TODO
 - [ ] Second Digit on speedo

## Output Designation
 - [x] Oil Temp > Outdoor Temp Degrees C (-5 > 40)
 - [x] Oil Temp Warning > Outdoor Temp < 0
 - [x] Battery Level > Home Storage Battery level %
 - [x] Battery Level Warning > Battery Level < 10%
 - [x] Coolant Temp > Indoor temp C (-5 > 40)
 - [x] Coolant Temp Warning > Indoor Temp < 15 degs
 - [x] Fuel Level > Electric car charge %
 - [x] Fuel Level warning > Electric car charge < 20%
 - [x] Speedo > How many tasks I have to do today (see input_number.active_todoist_jobs_today)
 - [x] RPM > Solar generation 0 > 7000


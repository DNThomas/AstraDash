# Astra Dash Online
Arduino code to use a ESP32 to control an Astra GTE Dashboard display.

## Initial wiring
1. Disconnect the blue connector from the main control board to the display board
1. Connect Dupont wires from pins 3,4,5,6,7 to ESP32 as per "Wiring" instructions

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

## Compile
``bin/arduino-cli compile --fqbn esp32:esp32:nodemcu-32s bitbang``

## Run
Note: Change ttyUSB1 to whatever port you are targeting.  ``sudo dmesg | grep tty`` to discover port.

``bin/arduino-cli upload -p /dev/ttyUSB1 --fqbn esp32:esp32:nodemcu-32s bitbang``

Or for those who will make lots of changes...

## Compile and Run
``./test.sh /dev/ttyUSB1``

## Notes
1. We don't use MISO as it response from Dash to Arduino.

## TODO
 - [ ] Do one gauge so I have working code that shows a certain value
 - [ ] Mask Bits so simple values can be written.
 - [ ] Design data structure IE if I want to write low battery level to fuel gauge I want to write something like "fuel,0"
 - [ ] Figure out a way to interact with the ESP32 to update the value of a given item

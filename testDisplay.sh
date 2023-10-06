#!/bin/bash
if test -z "$1"
then
      echo "TTY not set, assuming TTYUSB0 - if this doesn't work find your device TTY IE sudo dmesg | grep tty"
      TTY="/dev/ttyUSB0"
else
      TTY=$1
fi

bin/arduino-cli compile --fqbn esp32:esp32:nodemcu-32s testDisplay
bin/arduino-cli upload -p $TTY --fqbn esp32:esp32:nodemcu-32s testDisplay
bin/arduino-cli monitor -p $TTY --fqbn esp32:esp32:nodemcu-32s -c baudrate=115200


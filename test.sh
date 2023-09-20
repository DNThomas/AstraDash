bin/arduino-cli compile --fqbn esp32:esp32:nodemcu-32s bitbang
bin/arduino-cli upload -p $1 --fqbn esp32:esp32:nodemcu-32s bitbang


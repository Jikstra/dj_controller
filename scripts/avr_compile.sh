#!/bin/sh
PATH_TO_ARDUINO_AVR="/usr/share/arduino/hardware/archlinux-arduino/avr/"
avr-g++ -I $PATH_TO_ARDUINO_AVR/cores/arduino/ -I $PATH_TO_ARDUINO_AVR/variants/standard -x c++  -MMD -c -mmcu=atmega328p -Wall -DF_CPU=16000000L  -Wall  -Os $1

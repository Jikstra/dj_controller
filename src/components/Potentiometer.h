#ifndef POTENTIOMETER_H
#define POTENTIOMETER_H
#include <Arduino.h>
#include "../common.h"
#include "../midi.h"
#include "../debug.h"

class Potentiometer {
  const bool deck;
  unsigned long last_flake[2];
  bool was_pressed[2];
  bool toggle[2];
  public:
    int pin_potentiometer;
    int pin_button;
    int potentiometer_midi_value;
    float EMA_a;
    int EMA_S;

    Potentiometer(int pin_potentiometer, int pin_button, bool deck);
    void setup();
    void process();
    void _process_button(int pin_button_value);
    void _process_potentiometer(int pin_potentiometer_value);
};

#endif

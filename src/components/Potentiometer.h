#pragma once

#include <Arduino.h>
#include "./Component.h"
#include "../common.h"
#include "../midi.h"
#include "../debug.h"

const float EMA_ALPHA = 0.8;

class Potentiometer : public Component {
  Component* linked_components[10] = { NULL };
  unsigned short int id;
  unsigned long last_flake;
  bool was_pressed;
  bool toggle;
  public:
    int pin_potentiometer;
    int pin_button;
    int potentiometer_midi_value;
    int potentiometer_value_ema;

    Potentiometer(int pin_potentiometer, int pin_button, unsigned short int id);
    void setup();
    void process();
    void _process_button(int pin_button_value);
    void _process_potentiometer(int pin_potentiometer_value);
};

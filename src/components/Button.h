#pragma once

#include <Arduino.h>
#include "../common.h"
#include "../midi.h"
#include "../debug.h"

class Button {
  int control_number;
  const bool deck;
  unsigned long last_flake[2];
  bool was_pressed[2];
  bool toggle[2];
  public:
    int pin;

    Button(int pin, int control_number, bool deck);
    void setup();
    void process();
    void _process(int state);
};

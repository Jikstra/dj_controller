#ifndef BUTTON_H
#define BUTTON_H
#include <Arduino.h>
#include "../common.h"
#include "../debug.h"

class Button {
  unsigned long last_flake;
  bool was_pressed;
  public:
    int pin;

    Button(int pin);
    void setup();
    ButtonState process();
    ButtonState _process(int state);
};

#endif

#ifndef KNOB_H
#define KNOB_H
#include <Arduino.h>
#include <Rotary.h>

#include "common.h"

class RotaryEncoder {
  Rotary rotary;

  int button_pin;
  unsigned long button_last_flake;

  public:
    RotaryEncoder(int rotary_pin_a, int button_pin, int rotary_pin_b);
    void setup();
    void process();
    void processRotary();
    void processButton();
    void handleRotaryTurn(bool turnedLeft);
    void handleButtonPress(bool isPressed);
};

#endif

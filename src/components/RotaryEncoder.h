#ifndef KNOB_H
#define KNOB_H
#include <Arduino.h>
#include <Rotary.h>

#include "common.h"

class RotaryEncoder {

  public:
    Rotary rotary;

    int button_pin;
    bool button_was_pressed;
    bool button_toggle;
    unsigned long button_last_flake;

    RotaryEncoder(int rotary_pin_a, int button_pin, int rotary_pin_b);
    void setup();
    void process();
    void processRotary();
    void processButton();
    virtual void handleRotaryTurn(bool turnedLeft);
    virtual void handleButtonState(ButtonState buttonState);
    virtual void handleButtonToggle(bool toggle);
};

#endif

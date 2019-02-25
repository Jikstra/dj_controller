#ifndef BUTTON_H
#define BUTTON_H
#include <Arduino.h>
#include "../common.h"
#include "../midi.h"
#include "../debug.h"

class Button {
  int control_number;
  const bool deck;
  unsigned long last_flake;
  bool was_pressed;
  bool toggle;
  public:
    int pin;

    Button(int pin, int control_number, bool deck);
    void setup();
    void process();
    void _process(int state);
    virtual void handleButtonState(ButtonState button_state);
    virtual void handleButtonToggle(bool toggle); 
};

#endif

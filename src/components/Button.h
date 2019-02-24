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
  bool switch_is_up;
  bool switch_was_up;
  public:
    int pin;

    Button(int pin, int control_number, bool deck);
    void setup();
    void process();
    void _process(int state);
};

#endif

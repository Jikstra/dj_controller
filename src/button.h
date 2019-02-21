#ifndef BUTTON_H
#define BUTTON_H
#include "Arduino.h"

class Button {
  int pin;
  int control_number;
  const bool deck;
  unsigned long last_flake;
  bool switch_is_up;
  bool switch_was_up;
  public:
    Button(int pin, int control_number, bool deck);
    void setup();
    void loop();
    void _loop(int state);
};

#endif

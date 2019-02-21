#ifndef KNOB_H
#define KNOB_H
#include <Arduino.h>
#include <Rotary.h>

class Knob {
  Rotary rotary;
  int rotary_counter;

  int button_pin;
  bool button_is_pressed;
  bool button_was_pressed;
  unsigned long button_last_flake;

  int control_number_value;
  int control_number_mute;
  
  bool deck;
  int step_size;
  public:
    Knob(int rotary_pin_a, int button_pin, int rotary_pin_b, int control_number_value, int control_number_mute, bool deck, int step_size);
    void setup();
    void process();
    void processRotary();
    void processButton();
    int _getValueToSend();
};

#endif

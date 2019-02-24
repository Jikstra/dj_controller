#ifndef COUNTING_ROTARY_ENCODER_H
#define COUNTING_ROTARY_ENCODER_H
#include <Arduino.h>

#include "../common.h"
#include "../midi.h"
#include "../debug.h"
#include "RotaryEncoder.h"

class CountingRotaryEncoder : public RotaryEncoder {
  int rotary_counter;

  bool button_is_pressed;
  bool button_was_pressed;

  int control_number_value;
  int control_number_mute;
  
  bool deck;
  int step_size;
  public:
    CountingRotaryEncoder(int rotary_pin_a, int button_pin, int rotary_pin_b, int control_number_value, int control_number_mute, bool deck, int step_size);
    void handleRotaryTurn(bool turnedLeft);
    void handleButtonPress(bool isPressed);
    int _getValueToSend();
};

#endif

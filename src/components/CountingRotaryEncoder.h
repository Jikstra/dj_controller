#ifndef COUNTING_ROTARY_ENCODER_H
#define COUNTING_ROTARY_ENCODER_H
#include <Arduino.h>

#include "../common.h"
#include "../midi.h"
#include "../debug.h"
#include "RotaryEncoder.h"

class CountingRotaryEncoder : public RotaryEncoder {
  public:
    int rotary_counter[2];

    int control_number_value;
    int control_number_mute;
    
    bool deck;
    CountingRotaryEncoder(
      int rotary_pin_a, 
      int button_pin,
      int rotary_pin_b, 
      int control_number_value, 
      int control_number_mute, 
      bool deck 
    );
    CountingRotaryEncoder(
      int rotary_pin_a, 
      int button_pin,
      int rotary_pin_b, 
      int control_number_value, 
      int control_number_mute, 
      bool deck,
      int initial_count
    );
    void handleRotaryTurn(bool turnedLeft);
    void handleButtonToggle(bool toggle);
    int _getValueToSend(int counter);
};

#endif

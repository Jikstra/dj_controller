#ifndef LR_ROTARY_ENCODER_H
#define LR_ROTARY_ENCODER_H
#include <Arduino.h>

#include "../common.h"
#include "../midi.h"
#include "../debug.h"
#include "RotaryEncoder.h"

class LRRotaryEncoder : public RotaryEncoder {
  public:
    int rotary_counter;

    int control_number_left;
    int control_number_right;
    int control_number_press;
    int control_number_release;
    
    bool deck;

    LRRotaryEncoder(
      int rotary_pin_a, 
      int button_pin, 
      int rotary_pin_b, 
      int control_number_left, 
      int control_number_right, 
      int control_number_press, 
      int control_number_release, 
      bool deck
    );
    void handleRotaryTurn(bool turnedLeft);
    void handleButtonToggle(bool toggle);
};

#endif

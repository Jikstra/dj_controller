#ifndef POSITION_KNOB_H
#define POSITION_KNOB_H
#include <Arduino.h>

#include "../common.h"
#include "../midi.h"
#include "../debug.h"
#include "RotaryEncoder.h"

class PositionKnob : public RotaryEncoder {
  public:
    int rotary_counter;
    
    bool deck;

    const int midi_beatjump_forward_1 = 81;
    const int midi_beatjump_backward_1 = 82;
    const int midi_beatjump_forward_2 = 83;
    const int midi_beatjump_backward_2 = 84;
    const int midi_beatjump_forward_3 = 85;
    const int midi_beatjump_backward_3 = 86;
    const int midi_reloop_toggle = 33;
    const int midi_rate_temp_down_small = 65;
    const int midi_beats_translate_curpos = 66;
    const int midi_beatjump_forward_smaller = 67;
    const int midi_beatjump_backward_smaller = 68;

    PositionKnob(
      int rotary_pin_a, 
      int button_pin, 
      int rotary_pin_b, 
      bool deck
    );
    void handleRotaryTurn(bool turnedLeft);
    //virtual void handleButtonToggle(bool toggle);
    virtual void handleButtonState(ButtonState buttonState);
};

#endif

#ifndef LOOP_KNOB_H
#define LOOP_KNOB_H
#include <Arduino.h>

#include "../common.h"
#include "../midi.h"
#include "../debug.h"
#include "RotaryEncoder.h"

class LoopKnob : public RotaryEncoder {
  public:
    int rotary_counter;
    
    bool deck;

    const int midi_loop_halve = 37;
    const int midi_loop_double = 36;
    const int midi_loop_activate = 38;
    const int midi_rate_temp_up_small = 75;
    const int midi_beats_translate_curpos = 66;
    const int midi_beatjump_forward_small = 77;
    const int midi_beatjump_backward_small = 78;

    LoopKnob(
      int rotary_pin_a, 
      int button_pin, 
      int rotary_pin_b, 
      bool deck
    );
    void handleRotaryTurn(bool turnedLeft);
    virtual void handleButtonState(ButtonState buttonState);
};

#endif

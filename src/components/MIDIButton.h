#ifndef MIDIBUTTON_H
#define MIDIBUTTON_H
#include <Arduino.h>
#include "../common.h"
#include "../midi.h"
#include "../debug.h"
#include "./Button.h"

class MIDIButton {
  int control_number;
  const bool deck;
  Button buttons[2];
  public:
    int pin;
    bool toggles[2];
    MIDIButton(int p, int cn, bool d);
    void setup();
    void process();
    void _process(int pinState);
    virtual void handleButtonState(ButtonState button_state);
    virtual void handleButtonToggle(bool toggle);
};

#endif

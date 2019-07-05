#include "LRRotaryEncoder.h"

LRRotaryEncoder::LRRotaryEncoder
    (
      int rotary_pin_a, 
      int button_pin, 
      int rotary_pin_b, 
      int control_number_left, 
      int control_number_right, 
      int control_number_press, 
      int control_number_release, 
      bool deck
    ) : 
    RotaryEncoder(rotary_pin_a, button_pin, rotary_pin_b),
    
    control_number_left(control_number_left),
    control_number_right(control_number_right),
    control_number_press(control_number_press),
    control_number_release(control_number_release),
    
    deck(deck) {}

void LRRotaryEncoder::handleRotaryTurn(bool turnedLeft) {
  int channel = getChannelFromDeck(deck);
  int control_number = turnedLeft ? control_number_left : control_number_right;
  
  IFNDEBUG(
    // send a MIDI CC -- 56 = note, 127 = velocity, 1 = channel
    midiOut.sendControlChange(
      control_number,
      1,
      channel
    )
  );

  IFDEBUG(
    p("LRRotaryEncoder (BPM): %i:%i %i %s", control_number, channel, 1, turnedLeft ? "Left": "Right");  
  );
}

void LRRotaryEncoder::handleButtonToggle(bool toggle) {
  int channel = getChannelFromDeck(deck);
  int value_to_send = 1;
  int control_number = toggle ? control_number_press : control_number_release;

  IFDEBUG(
    p("Button: %i:%i %s %i", control_number, channel, toggle ? "Toggled" : "Untoggled", value_to_send)
  );

  IFNDEBUG(midiOut.sendNoteOn(control_number, value_to_send, channel));
}

#include "CountingRotaryEncoder.h"

CountingRotaryEncoder::CountingRotaryEncoder
    (
      int rotary_pin_a,
      int button_pin,
      int rotary_pin_b,
      int control_number_value,
      int control_number_mute,
      bool deck,
      int step_size
    ) : 
    RotaryEncoder(rotary_pin_a, button_pin, rotary_pin_b),
    rotary_counter(63),
   
    control_number_value(control_number_value),
    control_number_mute(control_number_mute),
    
    deck(deck),
    step_size(step_size) {}

int CountingRotaryEncoder::_getValueToSend() {
  int value_to_send = rotary_counter;
  if(value_to_send > 127) {
    value_to_send = 127;
  } else if(value_to_send < 0) {
    value_to_send = 0;
  }
  return value_to_send;  
}

void CountingRotaryEncoder::handleRotaryTurn(bool turnedLeft) {
  if (turnedLeft == true && rotary_counter < 127) {
    rotary_counter = rotary_counter + step_size;
  } else if(turnedLeft == false && rotary_counter > 0)  {
    rotary_counter = rotary_counter - step_size;
  } else {
    return;
  }

  int value_to_send = _getValueToSend();
  int channel = getChannelFromDeck(deck);
  
  IFDEBUG(
    p("Rotation: %i:%i %i", control_number_value, channel, value_to_send);  
  );
  IFNDEBUG(
    // send a MIDI CC -- 56 = note, 127 = velocity, 1 = channel
    midiOut.sendControlChange(
      control_number_value,
      value_to_send,
      channel
    )
  );
}

void CountingRotaryEncoder::handleButtonStateChange(bool isPressed) {
  int channel = getChannelFromDeck(deck);
  int value_to_send = isPressed ? 0 : 1;

  IFDEBUG(
    p("Button: %i:%i %s %i", control_number_mute, channel, isPressed ? "Pressed" : "Released", value_to_send)
  );

  IFNDEBUG(midiOut.sendNoteOn(control_number_mute, value_to_send, channel));
}


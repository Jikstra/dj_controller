#include "CountingRotaryEncoder.h"

CountingRotaryEncoder::CountingRotaryEncoder
    (
      int rotary_pin_a,
      int button_pin,
      int rotary_pin_b,
      int control_number_value,
      int control_number_mute,
      bool deck
    ) : 
    RotaryEncoder(rotary_pin_a, button_pin, rotary_pin_b),
    rotary_counter {63, 63},
   
    control_number_value(control_number_value),
    control_number_mute(control_number_mute),
    
    deck(deck)
    {}

int CountingRotaryEncoder::_getValueToSend(int counter) {
  int value_to_send = counter;
  if(value_to_send > 127) {
    value_to_send = 127;
  } else if(value_to_send < 0) {
    value_to_send = 0;
  }
  return value_to_send;  
}

void CountingRotaryEncoder::handleRotaryTurn(bool turnedLeft) {
  int channel = getChannelFromDeck(deck);
  int channelIndex = getUpperOrLowerChannelIndex(channel);

  unsigned int step_size = getStepSize();

  int counter = rotary_counter[channelIndex];
  if (turnedLeft == true) {
    counter = counter + step_size;
  } else if(turnedLeft == false)  {
    counter = counter - step_size;
  } else {
    return;
  }

  int value_to_send = _getValueToSend(counter);
  rotary_counter[channelIndex] = value_to_send;
  
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

void CountingRotaryEncoder::handleButtonToggle(bool toggle) {
  int channel = getChannelFromDeck(deck);
  int value_to_send = toggle;

  IFDEBUG(
    p("Button: %i:%i %s %i", control_number_mute, channel, toggle ? "Toggled" : "Untoggled", value_to_send)
  );

  IFNDEBUG(midiOut.sendNoteOn(control_number_mute, value_to_send, channel));
}


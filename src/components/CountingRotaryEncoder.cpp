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

CountingRotaryEncoder::CountingRotaryEncoder
    (
      int rotary_pin_a,
      int button_pin,
      int rotary_pin_b,
      int control_number_value,
      int control_number_mute,
      bool deck,
      int initial_count
    ) :
    RotaryEncoder(rotary_pin_a, button_pin, rotary_pin_b),
    rotary_counter {initial_count, initial_count},
   
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
  counter = turnedLeft ? counter - step_size : counter + step_size;

  int counterValue = _getValueToSend(counter);
  setCounter(channel, channelIndex, counterValue);
}

void CountingRotaryEncoder::setCounter(int channel, int channelIndex, int counterValue) {
  rotary_counter[channelIndex] = counterValue;
  
  IFDEBUG(
    p("Rotation: %i:%i %i", control_number_value, channel, counterValue);  
  );
  IFNDEBUG(
    // send a MIDI CC -- 56 = note, 127 = velocity, 1 = channel
    midiOut.sendControlChange(
      control_number_value,
      counterValue,
      channel
    )
  );

}

void CountingRotaryEncoder::handleButtonState(ButtonState button_state) {
  if (button_state == ButtonState::Unchanged) return;

  if (button_state == ButtonState::Pressed) {
    addPressedComponent(this);
  } else if (button_state == ButtonState::Unpressed) {

  }
  IFDEBUG(
    p("CountingRotaryEncoder ButtonState: %s", buttonStateToString(button_state))
  );
  if(buttonToggle(button_state, &button_toggle)) {
    handleButtonToggle(button_toggle);    
  }
}

void CountingRotaryEncoder::handleButtonToggle(bool toggle) {
  int channel = getChannelFromDeck(deck);
  int value_to_send = toggle;

  IFDEBUG(
    p("CountingRotaryEncoder Button: %i:%i %s %i", control_number_mute, channel, toggle ? "Toggled" : "Untoggled", value_to_send)
  );

  IFNDEBUG(midiOut.sendNoteOn(control_number_mute, value_to_send, channel));
}

void CountingRotaryEncoder::onPotentiometerChange(int midiValue) {
  int channel = getChannelFromDeck(deck);
  int channelIndex = getUpperOrLowerChannelIndex(channel);

  rotary_counter[channelIndex] = midiValue;
  IFDEBUG(
    p("CountingRotaryEncoder onPotentiometerChange: %i %i", control_number_value, midiValue)
  );
  setCounter(channel, channelIndex, midiValue);
}

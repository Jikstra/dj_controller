#include "VolumeCountingRotaryEncoder.h"

VolumeCountingRotaryEncoder::VolumeCountingRotaryEncoder
    (
      int rotary_pin_a,
      int button_pin,
      int rotary_pin_b,
      int control_number_value,
      int control_number_mute,
      bool deck
    ) : 
    CountingRotaryEncoder(rotary_pin_a, button_pin, rotary_pin_b, control_number_value, control_number_mute, deck, 127)
    {}



void VolumeCountingRotaryEncoder::handleButtonToggle(bool toggle) {
  int channel = getChannelFromDeck(deck);


  int value_to_send = 1;
  IFDEBUG(
    p("VolumeCountingRotaryEncoder Button: %i:%i %s %i", control_number_mute, channel, toggle ? "Toggled" : "Untoggled", value_to_send)
  );

  IFNDEBUG(midiOut.sendNoteOn(control_number_mute, value_to_send, channel));

  int channelIndex = getUpperOrLowerChannelIndex(channel);
  
  rotary_counter[channelIndex] = 0;
  
  IFDEBUG(
    p("VolumeCountingRotaryEncoder Rotation: %i:%i %i", control_number_value, channel, 0);  
  );
  IFNDEBUG(
    // send a MIDI CC -- 56 = note, 127 = velocity, 1 = channel
    midiOut.sendControlChange(
      control_number_value,
      0,
      channel
    )
  );
}

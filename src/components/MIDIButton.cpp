#include "MIDIButton.h"

MIDIButton::MIDIButton(int pin, int control_number, bool deck) :
  control_number(control_number),
  deck(deck),
  buttons { Button(pin), Button(pin) },
  toggles { 0, 0 } {}

void MIDIButton::setup() {
  buttons[0].setup();
  buttons[1].setup();
}

void MIDIButton::process() {
  int channelIndex = getChannelIndexFromDeck(deck);
  ButtonState button_state = buttons[channelIndex].process();
  handleButtonState(button_state);
}


void MIDIButton::_process(int state) {
  int channelIndex = getChannelIndexFromDeck(deck);
  ButtonState button_state = buttons[0]._process(state);
  p("ButtonState %s", buttonStateToString(button_state));

  handleButtonState(button_state);
}

void MIDIButton::handleButtonState(ButtonState button_state) {
  int channelIndex = getChannelIndexFromDeck(deck);
  if(buttonToggle(button_state, &toggles[channelIndex])) {
    handleButtonToggle(toggles[channelIndex]);    
    p("Hallo3");
  }
}

void MIDIButton::handleButtonToggle(bool toggle) {
  int value_to_send = 1;
  int channel = getChannelFromDeck(deck);

  IFDEBUG(p(
    "MIDIButton: %i:%i %s %i",
    control_number,
    channel,
    toggle ? "Toggled" : "Untoggled",
    value_to_send
  ));

  IFNDEBUG(midiOut.sendNoteOn(control_number, value_to_send, channel));
}

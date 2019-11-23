#include "Button.h"

Button::Button(int pin, int control_number, bool deck) :
  control_number(control_number),
  deck(deck),
  last_flake {0, 0},
  was_pressed {false, false},
  toggle {false, false},
  pin(pin) {}


void Button::setup() {
  pinMode(pin, INPUT_PULLUP);
}

void Button::process() {
  int state = digitalRead(pin);
  _process(state);
}

void Button::_process(int pin_value) {
  int channel = getChannelFromDeck(deck);
  int channelIndex = getUpperOrLowerChannelIndex(channel);

  ButtonState button_state = buttonState(pin_value, &was_pressed[channelIndex], &last_flake[channelIndex]);

  if(!buttonToggle(button_state, &toggle[channelIndex])) {
    return;
  }

  int value_to_send = 1;

  IFDEBUG(p(
    "Button: %i:%i %s %i",
    control_number,
    channel,
    toggle ? "Toggled" : "Untoggled",
    value_to_send
  ));

  IFNDEBUG(midiOut.sendNoteOn(control_number, value_to_send, channel));
}

#include "Button.h"

Button::Button(int pin, int control_number, bool deck) :
  control_number(control_number),
  deck(deck),
  last_flake(0),
  was_pressed(false),
  toggle(false),
  pin(pin) {}


void Button::setup() {
  pinMode(pin, INPUT_PULLUP);
}

void Button::process() {
  int state = digitalRead(pin);
  _process(state);
}

void Button::_process(int pin_value) {
  ButtonState button_state = buttonState(pin_value, &was_pressed, &last_flake);

  handleButtonState(button_state);
}

void Button::handleButtonState(ButtonState button_state) {
  if(buttonToggle(button_state, &toggle)) {
    handleButtonToggle(toggle);    
  }
}

void Button::handleButtonToggle(bool toggle) {
  int value_to_send = 1;
  int channel = getChannelFromDeck(deck);

  IFDEBUG(p(
    "Button: %i:%i %s %i",
    control_number,
    channel,
    toggle ? "Toggled" : "Untoggled",
    value_to_send
  ));

  IFNDEBUG(midiOut.sendNoteOn(control_number, value_to_send, channel));
}

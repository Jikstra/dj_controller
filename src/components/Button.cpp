#include "Button.h"

Button::Button(int pin) :
  last_flake(0),
  was_pressed(false),
  pin(pin) {}


void Button::setup() {
  pinMode(pin, INPUT_PULLUP);
}

ButtonState Button::process() {
  int state = digitalRead(pin);
  return _process(state);
}

ButtonState Button::_process(int pin_value) {
  ButtonState button_state = buttonState(pin_value, &was_pressed, &last_flake);
  return button_state;
}

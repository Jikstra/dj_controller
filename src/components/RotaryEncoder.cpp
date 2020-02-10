#include "RotaryEncoder.h"

RotaryEncoder::RotaryEncoder(
    int rotary_pin_a,
    int button_pin,
    int rotary_pin_b
  ) : 
    rotary(Rotary(rotary_pin_a, rotary_pin_b)),
    button_pin(button_pin),
    button_was_pressed(false),
    button_toggle(false),
    button_last_flake(0) {}


void RotaryEncoder::setup() {
  rotary.begin(true);
  pinMode(button_pin, INPUT_PULLUP);
}

void RotaryEncoder::process() {
  processRotary();
  processButton();
}

void RotaryEncoder::processRotary() {
  unsigned char result = rotary.process();
  
  if (result == DIR_NONE) return;
  handleRotaryTurn(result == DIR_CW);
}

void RotaryEncoder::handleRotaryTurn(bool turnedLeft) {}

void RotaryEncoder::processButton() {
  int pinValue = !digitalRead(button_pin); 
  ButtonState button_state = buttonState(pinValue, &button_was_pressed, &button_last_flake);
  IFDEBUG(
    if (button_state != ButtonState::Unchanged) {
      p(
        "RotaryEncoder: state: %s",
        buttonStateToString(button_state)
      );
    }
  );

  handleButtonState(button_state);
}

void RotaryEncoder::handleButtonState(ButtonState button_state) {
  if(buttonToggle(button_state, &button_toggle)) {
    handleButtonToggle(button_toggle);    
  }
}

void RotaryEncoder::handleButtonToggle(bool toggle) {
}





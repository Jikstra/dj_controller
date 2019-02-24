#include "RotaryEncoder.h"

RotaryEncoder::RotaryEncoder(
    int rotary_pin_a,
    int button_pin,
    int rotary_pin_b
  ) : 
    rotary(Rotary(rotary_pin_a, rotary_pin_b)),
    button_pin(button_pin),
    button_last_flake(0),
    button_is_pressed(false),
    button_was_pressed(false) {}


void RotaryEncoder::setup() {
  pinMode(button_pin, INPUT_PULLUP);
}

void RotaryEncoder::process() {
  processRotary();
  processButton();
}

void RotaryEncoder::processRotary() {
  int result = rotary.process();


  if (result == DIR_NONE) return;

  handleRotaryTurn(result == DIR_CW);
}

void RotaryEncoder::handleRotaryTurn(bool turnedLeft) {}

void RotaryEncoder::processButton() {
  int button = digitalRead(button_pin);

  if(isBouncing(&button_last_flake)) return;
  handleButtonPress(button == HIGH);
}

void RotaryEncoder::handleButtonPress(bool isHigh) {
  if(!isHigh && button_was_pressed == false) {
    button_is_pressed = !button_is_pressed;
    button_was_pressed = true;
  } else if(isHigh && button_was_pressed == true){
    button_was_pressed = false;
    return;
  } else {
    return;
  }

  handleButtonStateChange(button_is_pressed);
}

void RotaryEncoder::handleButtonStateChange(bool isPressed) {

}




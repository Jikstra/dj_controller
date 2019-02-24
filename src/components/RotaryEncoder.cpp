#include "RotaryEncoder.h"

RotaryEncoder::RotaryEncoder(int rotary_pin_a, int button_pin, int rotary_pin_b) : 
    rotary(Rotary(rotary_pin_a, rotary_pin_b)),
    button_pin(button_pin),
    button_last_flake(0) {}

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
  handleRotaryTurn(button == HIGH);
}

void RotaryEncoder::handleButtonPress(bool isPressed) {}

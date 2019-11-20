#include "Potentiometer.h"

Potentiometer::Potentiometer(int pin_potentiometer, int pin_button, unsigned short int id) :
  id(id),
  last_flake(0),
  was_pressed(false),
  toggle(false),
  pin_potentiometer(pin_potentiometer),
  pin_button(pin_button),
  potentiometer_midi_value(-1),
  potentiometer_value_ema(0) {}


void Potentiometer::setup() {
  pinMode(pin_button, INPUT_PULLUP);
  potentiometer_value_ema = analogRead(pin_potentiometer);
}

void Potentiometer::process() {
  int pin_button_value = digitalRead(pin_button);
  _process_button(pin_button_value);

  int sensorValue = analogRead(pin_potentiometer); 
  potentiometer_value_ema = (EMA_ALPHA*sensorValue) + ((1-EMA_ALPHA)*potentiometer_value_ema);
  _process_potentiometer(potentiometer_value_ema);
}

void Potentiometer::_process_button(int pin_button_value) {
  ButtonState button_state = buttonState(pin_button_value, &was_pressed, &last_flake);

  if(!buttonToggle(button_state, &toggle)) {
    return;
  }

  int value_to_send = 1;

  IFDEBUG(p(
    "Potentiometer Button: %i %s %i",
    id,
    toggle ? "Toggled" : "Untoggled",
    value_to_send
  ));
}

void Potentiometer::_process_potentiometer(int pin_button_value) {
  int midi_value = pin_button_value / 8;

  if (midi_value == potentiometer_midi_value) {
      return;
  }

  potentiometer_midi_value = midi_value;


  Component** pressedComponents = getPressedComponents();
  for (int i = 0; i<_COUNT_PRESSED_COMPONENTS; i++) {
    if (_PRESSED_COMPONENTS[i] == NULL) {
      break;
    }
    pressedComponents[i]->onPotentiometerChange(potentiometer_midi_value);
  }

  IFDEBUG(p(
    "Potentiometer Rotation: %i %i %i",
    id,
    midi_value,
    pin_button_value
  ));
}

#include "Potentiometer.h"

Potentiometer::Potentiometer(int pin_potentiometer, int pin_button, bool deck) :
  deck(deck),
  last_flake {0, 0},
  was_pressed {false, false},
  toggle {false, false},
  pin_potentiometer(pin_potentiometer),
  pin_button(pin_button),
  potentiometer_midi_value(0),
  EMA_a(0.8),
  EMA_S(0) {}


void Potentiometer::setup() {
  pinMode(pin_button, INPUT_PULLUP);
  EMA_S = analogRead(pin_potentiometer);
}

void Potentiometer::process() {
  int pin_button_value = digitalRead(pin_button);
  _process_button(pin_button_value);

  int sensorValue = analogRead(pin_potentiometer); 
  EMA_S = (EMA_a*sensorValue) + ((1-EMA_a)*EMA_S);
  _process_potentiometer(EMA_S);
}

void Potentiometer::_process_button(int pin_button_value) {
  int channel = getChannelFromDeck(deck);
  int channelIndex = getUpperOrLowerChannelIndex(channel);
  ButtonState button_state = buttonState(pin_button_value, &was_pressed[channelIndex], &last_flake[channelIndex]);

  if(!buttonToggle(button_state, &toggle[channelIndex])) {
    return;
  }

  int value_to_send = 1;

  IFDEBUG(p(
    "Potentiometer Button: %i %s %i",
    channel,
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

  int channel = getChannelFromDeck(deck);
  int channelIndex = getUpperOrLowerChannelIndex(channel);

  IFDEBUG(p(
    "Potentiometer Potentiometer: %i %i %i",
    channel,
    midi_value,
    pin_button_value
  ));
}

#include "Potentiometer.h"

Potentiometer::Potentiometer(int pin_potentiometer, int pin_button, bool deck) :
  deck(deck),
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

  IFDEBUG(p(deck ? "true" : "false")); 
}

void Potentiometer::process() {
  int pin_button_value = digitalRead(pin_button);
  _process_button(pin_button_value);

  int sensorValue = analogRead(pin_potentiometer); 
  potentiometer_value_ema = (EMA_ALPHA*sensorValue) + ((1-EMA_ALPHA)*potentiometer_value_ema);
  _process_potentiometer(potentiometer_value_ema);
}

void Potentiometer::_process_button(int pin_button_value) {
  int channel = getChannelFromDeck(deck);
  ButtonState button_state = buttonState(pin_button_value, &was_pressed, &last_flake);

  if(!buttonToggle(button_state, &toggle)) {
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

  IFDEBUG(p(
    "Potentiometer Rotation: %i %i %i",
    channel,
    midi_value,
    pin_button_value
  ));

  IFNDEBUG(
    // send a MIDI CC -- 56 = note, 127 = velocity, 1 = channel
    midiOut.sendControlChange(
      15,
      potentiometer_midi_value,
      channel
    )
  );
}

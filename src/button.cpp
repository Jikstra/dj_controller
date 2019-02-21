#include "button.h"
#include "common.h"
#include "midi.h"
#include "debug.h"

Button::Button(int pin, int control_number, bool deck) :
  pin(pin),
  control_number(control_number),
  deck(deck),
  last_flake(0),
  switch_is_up(true),
  switch_was_up(true) {}


void Button::setup() {
  pinMode(pin, INPUT_PULLUP);
}

void Button::loop() {
  int state = digitalRead(pin);
  _loop(state);
}

void Button::_loop(int state) {
  if(state == LOW && switch_was_up == false) {
    if(isBouncing(&last_flake)) return;
    switch_is_up = !switch_is_up;
    switch_was_up = true;
  } else if(state == HIGH && switch_was_up == true){
    if(isBouncing(&last_flake)) return;
    switch_was_up = false;
    return;
  } else {
    return;
  }

  int value_to_send = 1;
  int channel = getChannelFromDeck(deck);

  IFDEBUG(p(
    "Button: %i:%i %s %i",
    control_number,
    channel,
    value_to_send ? "Up" : "Down",
    value_to_send
  ));

  IFNDEBUG(midiOut.sendNoteOn(control_number, value_to_send, channel));

}

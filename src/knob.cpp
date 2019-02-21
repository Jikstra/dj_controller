#include "knob.h"
#include "common.h"
#include "midi.h"
#include "debug.h"

Knob::Knob(int rotary_pin_a, int button_pin, int rotary_pin_b, int control_number_value, int control_number_mute, bool deck, int step_size) : 
    rotary(Rotary(rotary_pin_a, rotary_pin_b)),
    rotary_counter(63),
    
    button_pin(button_pin),
    button_is_pressed(false),
    button_was_pressed(false),
    button_last_flake(0),
    
    control_number_value(control_number_value),
    control_number_mute(control_number_mute),
    
    deck(deck),
    step_size(step_size) {}

void Knob::setup() {
  pinMode(button_pin, INPUT_PULLUP);
}

void Knob::process() {
  processRotary();
  processButton();
}

void Knob::processRotary() {
  int result = rotary.process();

  if (result == DIR_NONE) return;

  if (result == DIR_CCW && rotary_counter < 127) {
    rotary_counter = rotary_counter + step_size;
  } else if(result == DIR_CW && rotary_counter > 0)  {
    rotary_counter = rotary_counter - step_size;
  } else {
    return;
  }

  int value_to_send = _getValueToSend();
  int channel = getChannelFromDeck(deck);
  
  IFDEBUG(
    p("Rotation: %i:%i %i", control_number_value, channel, value_to_send);  
  );
  IFNDEBUG(
    // send a MIDI CC -- 56 = note, 127 = velocity, 1 = channel
    midiOut.sendControlChange(
      control_number_value,
      value_to_send,
      channel
    )
  );
}

void Knob::processButton() {
  int button = digitalRead(button_pin);
  
  /*if(button == LOW && button_is_pressed == false) {
    button_is_pressed = true;
  } else if(button == HIGH && button_is_pressed == true) {
    button_is_pressed = false;
  } else {
    return;
  }*/
  
  if(button == LOW && button_was_pressed == false) {
    if(isBouncing(&button_last_flake)) return;
    button_is_pressed = !button_is_pressed;
    button_was_pressed = true;
  } else if(button == HIGH && button_was_pressed == true){
    if(isBouncing(&button_last_flake)) return;
    button_was_pressed = false;
    return;
  } else {
    return;
  }

  int channel = getChannelFromDeck(deck);
  int value_to_send = button_is_pressed ? 0 : 1;

  IFDEBUG(
    p("Button: %i:%i %s %i", control_number_mute, channel, button_is_pressed ? "Pressed" : "Released", value_to_send)
  );

  IFNDEBUG(midiOut.sendNoteOn(control_number_mute, value_to_send, channel));
}

int Knob::_getValueToSend() {
  int value_to_send = rotary_counter;
  if(value_to_send > 127) {
    value_to_send = 127;
  } else if(value_to_send < 0) {
    value_to_send = 0;
  }
  return value_to_send;  
}

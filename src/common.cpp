#include "common.h"

int channel_deck_a = 0;
int channel_deck_b = 0;


void p(char *fmt, ... ){
  char buf[128]; // resulting string limited to 128 chars
  va_list args;
  va_start (args, fmt );
  vsnprintf(buf, 128, fmt, args);
  va_end (args);
  Serial.println(buf);
}

int getChannelFromDeck(bool deck) {
  if(deck == DECK_A) {
    return channel_deck_a;
  } else {
    return channel_deck_b;
  }
}

void setChannelForDeck(bool deck, int channel) {
  if((deck == DECK_A && channel_deck_a == channel) || (deck == DECK_B && channel_deck_b == channel)) return;
  IFDEBUG(p("DECK %s = %i", deck == DECK_A ? "A" : "B", channel));
  if(deck == DECK_A) {
    channel_deck_a = channel;
  } else {
    channel_deck_b = channel;
  }
}

bool isBouncing(unsigned long* last_flake_millis) {
  unsigned long current_flake = millis();
  //p("current_flake %lu last_flake %lu", current_flake, *last_flake_millis);
  bool returnValue = current_flake - *last_flake_millis < 50;
  *last_flake_millis = current_flake;
  return returnValue;  
}


ButtonState buttonState(int pin_value, bool* was_pressed,  unsigned long* last_flake_millis) {
  if(pin_value == LOW && *was_pressed == false) {
    if(isBouncing(last_flake_millis)) return;
    *was_pressed = true;
    return ButtonState::Pressed;
  } else if(pin_value == HIGH && *was_pressed == true){
    if(isBouncing(last_flake_millis)) return;
    *was_pressed = false;
    return ButtonState::Unpressed;
  } else {
    return ButtonState::Unchanged;
  }
}

char* buttonStateToString(ButtonState button_state) {
  switch(button_state) {
    case ButtonState::Pressed:
      return "Pressed";
    case ButtonState::Unchanged:
      return "Unchanged";
    case ButtonState::Unpressed:
      return "Unpressed";
    default:
      return "Invalid Enum";
  }
}

bool buttonToggle(ButtonState button_state, bool* toggle) {
  if(button_state == ButtonState::Pressed) {
    *toggle = !*toggle;
    return true;
  }
  return false;
}

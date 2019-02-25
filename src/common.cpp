#include "common.h"

int channel_deck_a = 1;
int channel_deck_b = 2;


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
    return 1;
  } else {
    return 0;
  }
}

bool isBouncing(unsigned long* lastFlakeMillis) {
  unsigned long current_flake = millis();
  //p("current_flake %lu last_flake %lu", current_flake, *lastFlakeMillis);
  bool returnValue = current_flake - *lastFlakeMillis < 50;
  *lastFlakeMillis = current_flake;
  return returnValue;  
}


ButtonState buttonState(int pinValue, bool* wasPressed,  unsigned long* lastFlakeMillis) {
  if(pinValue == LOW && *wasPressed == false) {
    if(isBouncing(lastFlakeMillis)) return;
    *wasPressed = true;
    return ButtonState::Pressed;
  } else if(pinValue == HIGH && *wasPressed == true){
    if(isBouncing(lastFlakeMillis)) return;
    *wasPressed = false;
    return ButtonState::Unpressed;
  } else {
    return ButtonState::Unchanged;
  }
}

char* buttonStateToString(ButtonState buttonState) {
  switch(buttonState) {
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

bool buttonToggle(ButtonState buttonState, bool* toggle) {
  if(buttonState == ButtonState::Pressed) {
    *toggle = !*toggle;
    return true;
  }
  return false;
}

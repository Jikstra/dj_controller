#ifndef COMMON_H
#define COMMON_H

#include <Arduino.h>
#include "debug.h"

const bool DECK_A = 0;
const bool DECK_B = 1;

extern int channel_deck_a;
extern int channel_deck_b;

void p(char *fmt, ... );
int getChannelFromDeck(bool deck);
void setChannelForDeck(bool deck, int channel);
int getUpperOrLowerChannelIndex(int channel);
int getUpperOrLowerChannelIndexFromDeck(bool deck);
bool isBouncing(unsigned long* last_flake_millis);

enum class ButtonState {Pressed, Unchanged, Unpressed};

ButtonState buttonState(int pinValue, bool* wasPressed,  unsigned long* lastFlakeMillis);
char* buttonStateToString(ButtonState buttonState);
bool buttonToggle(ButtonState buttonState, bool* toggle);

#endif

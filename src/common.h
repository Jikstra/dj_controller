#ifndef COMMON_H
#define COMMON_H

#include <Arduino.h>
#include "debug.h"
#include "components/Component.h"

const bool DECK_A = 0;
const bool DECK_B = 1;

extern int channel_deck_a;
extern int channel_deck_b;

extern unsigned int step_size;

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


void setStepSize(unsigned int new_step_size);
unsigned int getStepSize();

extern Component* _PRESSED_COMPONENTS[10];
extern int _COUNT_PRESSED_COMPONENTS;

void addPressedComponent(Component* component);
void removePressedComponent(Component* component);
Component** getPressedComponents();
void debugPressedComponents();
void _debugPressedComponents(Component** components);

#endif

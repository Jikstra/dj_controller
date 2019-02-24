#ifndef COMMON_H
#define COMMON_H

#include "Arduino.h"

const bool DECK_A = 0;
const bool DECK_B = 1;

extern int channel_deck_b;
extern int channel_deck_b;

void p(char *fmt, ... );
int getChannelFromDeck(bool deck);
bool isBouncing(unsigned long* last_flake_millis);

#endif
